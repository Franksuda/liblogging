# Set-StrictMode -Version Latest
# $ErrorActionPreference = "Stop"
# $PSDefaultParameterValues['*:ErrorAction']='Stop'
# function ThrowOnNativeFailure {
#     if (-not $?)
#     {
#         throw 'Native Failure'
#     }
# }

Remove-Item -Recurse -Force "build.x86"
New-Item -Name "build.x86" -ItemType Directory -Force
Set-Location build.x86

cmake -E env CFLAGS="/D_WIN32_WINNT=0x0501 /MP" CXX_FLAGS="/D_WIN32_WINNT=0x0501 /MP" cmake -Wno-dev -G "Visual Studio 16 2019" -T v141_xp -A Win32 ..
cmake --build . --config Release -j

Set-Location ..

Remove-Item -Recurse -Force "build.x64"
New-Item -Name "build.x64" -ItemType Directory -Force
Set-Location build.x64

# cmake -E env CFLAGS="/D_WIN32_WINNT=0x0501" CXX_FLAGS="/D_WIN32_WINNT=0x0501" cmake -Wno-dev -G "Visual Studio 16 2019" -T v141_xp -A x64 ..
cmake -E env CFLAGS="/MP" CXX_FLAGS="/MP" cmake ..
cmake --build . --config Release -j