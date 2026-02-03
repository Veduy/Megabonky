<!-- .github/copilot-instructions.md - guidance for AI coding agents working on Megabonky -->
# Megabonky — Copilot guidance

Purpose: give coding agents the essential, actionable knowledge to be productive in this Unreal Engine project.

- **Engine:** This project targets Unreal Engine 5.7 (see [Megabonky.uproject](Megabonky.uproject)).
- **Primary languages:** C++ (module `Megabonky`) + Blueprints/UAssets (heavy asset usage under `Content/` and `Blueprints/`).

Quick start (build & run)
- **Open in Visual Studio:** open `Megabonky.sln` and build the `Editor` target. Use the `.vsconfig` components if replicating the dev environment.
- **Run the editor:** start your installed UE 5.7 `UnrealEditor.exe` pointing at `Megabonky.uproject` and pass `-log` for detailed output. Example template:

```
"{UE_INSTALL_DIR}/Engine/Binaries/Win64/UnrealEditor.exe" "C:/Work/Megabonky/Megabonky.uproject" -log
```

Project structure & important files
- `Source/Megabonky/` — main C++ module. See `Megabonky.Build.cs` for module dependencies (GameplayAbilities, Niagara, HTTP, Json).
- `Source/Megabonky/MgbSubsystem.*` — example GameInstance subsystem using `FHttpModule` to call a local backend (`http://127.0.0.1:8080/api/...`). Use as canonical example for HTTP integration.
- `Plugins/MgbDebugTool/` — editor plugin included in the uproject; treat it as an editor-only integration point.
- `Blueprints/` and `Content/` — most gameplay logic is exposed via Blueprints/Assets. Do not attempt to edit `.uasset` files directly outside the Editor.
- `Maps/` — entry maps such as `Game.umap` and `Title.umap` are where runtime scenes and startup flows live.

Conventions and patterns to follow
- Mixed C++ + Blueprint workflow: implement systems in C++ (in `Source/Megabonky/`) when they are engine-level subsystems; expose interfaces/events to designers via Blueprints under `Blueprints/`.
- Naming: project-prefixed identifiers often use `Mgb` or `Megabonky` (e.g., `UMgbSubsystem`, `MgbDebugTool`). Follow existing capitalization and UE style.
- Subsystems: global game logic lives in UE Subsystems (see `UMgbSubsystem`) — use subsystem lifetimes for cross-map state.
- Ability system: `GameplayAbilities` and `GameplayTags` are added in `Megabonky.Build.cs`; look under `Blueprints/AbilitySystem` and `TEST/` for existing examples/tests.

Integration notes (concrete examples)
- Backend calls: `UMgbSubsystem::Login()` issues a GET to `127.0.0.1:8080/api/login` — when implementing or testing network features, ensure a local test server is available or mock `FHttpModule` requests.
- Plugins: `MgbDebugTool` is enabled in the uproject and provides editor tooling — changes here are editor-only and may require rebuilding Editor modules.

What agents should and should not do
- Do: modify C++ under `Source/`, update `Megabonky.Build.cs` for module dependencies, and create Blueprint edits through the Editor workflow.
- Do: update `Megabonky.sln` by regenerating project files after adding new modules/classes (use the Editor or right-click `.uproject` → Generate Visual Studio project files).
- Don't: edit `.uasset` binary files directly outside Unreal Editor. Avoid assuming runtime engine install paths — use `{UE_INSTALL_DIR}` placeholders in commands.

Where to look for help
- Code entry points: `Source/Megabonky/Megabonky.cpp`, `Source/Megabonky/Megabonky.h`, `Source/Megabonky/MgbSubsystem.*`.
- Plugin definition: `Plugins/MgbDebugTool/MgbDebugTool.uplugin` and its `Source/` folder.

If you make changes
- When adding native modules or editor plugins, regenerate project files and rebuild the solution.
- Run the Editor with `-log` to capture runtime errors in `Saved/Logs/`.

Questions for maintainers (ask the user):
- Preferred Visual Studio version and CI build agent image? (`.vsconfig` suggests VS with NativeGame workload.)
- Is the local backend at `127.0.0.1:8080` a stable dev dependency or a transient test fixture?

If anything here is incomplete or incorrect, tell me what to clarify and I will update this file.
