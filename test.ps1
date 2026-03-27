$ErrorActionPreference = "Stop"

function Invoke-Step {
    param(
        [string]$Title,
        [scriptblock]$Action
    )

    Write-Host ""
    Write-Host "== $Title =="
    & $Action
}

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $projectRoot "build"
$testsExePath = Join-Path $projectRoot "build\\bin\\Debug\\tool_hook_tests.exe"

Invoke-Step "Build Debug binaries" {
    & cmake --build $buildDir --config Debug
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed with exit code $LASTEXITCODE"
    }
}

if (-not (Test-Path $testsExePath)) {
    throw "Tests binary not found after build: $testsExePath"
}

$toolKitRoot = Join-Path ${env:ProgramFiles(x86)} "Steam\steamapps\common\H3EK"

Invoke-Step "Run hook harness tests (launcher + XML)" {
    Push-Location $projectRoot
    try {
        & $testsExePath $toolKitRoot
        if ($LASTEXITCODE -ne 0) {
            throw "tool_hook_tests.exe failed with exit code $LASTEXITCODE"
        }
    }
    finally {
        Pop-Location
    }
}
