$ErrorActionPreference = "Stop"

$TaskName = "cypress_server"
$ProcessName = "cypress"
$Source = ".\cypress_update.exe"
$Destination = ".\cypress.exe"

function Fail($Message, $Code = 1) {
    Write-Host "[ERROR] $Message" -ForegroundColor Red
    exit $Code
}

try {
    Stop-ScheduledTask -TaskName $TaskName -ErrorAction Stop

    $task = Get-ScheduledTask -TaskName $TaskName -ErrorAction Stop
    if ($task.State -ne "Ready") {
        Fail "Scheduled task did not stop. Current state: $($task.State)" 10
    }

    Write-Host "[OK] Scheduled task stopped."
}
catch {
    Fail "Failed to stop scheduled task: $($_.Exception.Message)" 10
}

try {
    $processes = Get-Process -Name $ProcessName -ErrorAction SilentlyContinue

    if ($processes) {
        $processes | Stop-Process -Force -ErrorAction Stop

        # Verify process termination
        Start-Sleep -Milliseconds 500
        $remaining = Get-Process -Name $ProcessName -ErrorAction SilentlyContinue

        if ($remaining) {
            Fail "Process '$ProcessName' is still running." 20
        }
    }

    Write-Host "[OK] Process stopped or was not running."
}
catch {
    Fail "Failed to stop process: $($_.Exception.Message)" 20
}

try {
    if (-not (Test-Path -LiteralPath $Source -PathType Leaf)) {
        Fail "Source file not found: $Source" 30
    }

    if (Test-Path -LiteralPath $Destination) {
        Remove-Item -LiteralPath $Destination -Force -ErrorAction Stop
    }

    Move-Item -LiteralPath $Source -Destination $Destination -ErrorAction Stop

    if (-not (Test-Path -LiteralPath $Destination -PathType Leaf)) {
        Fail "Destination file was not created: $Destination" 30
    }

    Write-Host "[OK] File replaced successfully."
}
catch {
    Fail "Failed to replace file: $($_.Exception.Message)" 30
}

try {
    Start-ScheduledTask -TaskName $TaskName -ErrorAction Stop

    $task = Get-ScheduledTask -TaskName $TaskName -ErrorAction Stop
    if ($task.State -eq "Disabled") {
        Fail "Scheduled task is disabled and did not start." 40
    }

    Write-Host "[OK] Scheduled task start requested."
}
catch {
    Fail "Failed to start scheduled task: $($_.Exception.Message)" 40
}

Write-Host "[OK] All operations completed." -ForegroundColor Green
exit 0