param(
    [string]$error_dir
)

Write-Host "Path: $error_dir"

if (Test-Path -Path $error_dir -PathType Container) {
    Write-Host "Error dir: $error_dir exists"
    Remove-Item -Path $error_dir -Recurse
} else {
    Write-Host "Error dir: $error_dir does not exist"
}
