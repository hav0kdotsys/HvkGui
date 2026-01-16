# HvkGui
Custom ImGui Framework.

## Build helper
Use `build.ps1` to build one or more configurations via MSBuild, matching Visual Studio's build process.

```powershell
# Build Debug
./build.ps1 -debug

# Build Release with verbose output
./build.ps1 -release -verbose

# Build multiple configurations (including custom ones)
./build.ps1 -configurations Debug,Release,MyCustomConfig

# Override MSBuild path or platform
./build.ps1 -debug -msbuildPath "D:\Program Files\Microsoft Visual Studio\18\Insiders\MSBuild\Current\Bin\MSBuild.exe" -platform x64
```
