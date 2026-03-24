# Fileforge Roadmap

Fileforge starts as a reliable file-operations engine and grows into a polished Windows desktop app with installable releases, fast previews, and optional AI-assisted sorting.

## Product Goal

A Windows-first file organization app that lets users:
- scan a folder tree without moving data yet
- build an internal understanding of what is inside
- preview a proposed organization plan
- review and adjust that plan
- apply changes safely with rollback support
- optionally plug in an AI model for smarter sorting suggestions

## Core Product Principles

1. **Preview first, move later**  
   Fileforge should not move gigabytes of data just to show a preview. It should scan the folder tree, build a model of the current state, and generate a proposed future structure before any file operations happen.

2. **Engine/UI separation**  
   The file analysis and planning engine should live separately from the interface so the same core can power CLI tooling, automated tests, and eventually the Windows desktop UI.

3. **Windows-first product quality**  
   The long-term target is a normal Windows app with an installer, versioned releases, icons, and a familiar desktop workflow.

4. **AI should be optional**  
   Fileforge must be useful without AI. AI becomes a higher-level planner that proposes sorting logic on top of the scanned file inventory.

## Architecture Direction

## 1. Scan Engine
Build a recursive scanner that walks folders and produces a structured inventory of the file tree.

Each file/folder record should eventually include fields like:
- absolute path
- relative path
- filename
- extension
- size
- modified time
- file type/category
- hash (optional/later)
- parent folder
- inferred tags/metadata (later)

This becomes the internal "data table" / inventory model for previews and sort planning.

## 2. Planning Engine
Build a planner that takes the scanned inventory and produces a **proposed organization plan** without touching files.

The plan should include:
- current path
- proposed path
- reason/rule used
- conflict warnings
- skipped files
- summary counts

This is the heart of fast previews.

## 3. Apply Engine
Once a plan is approved, the apply step performs the real file operations:
- move/rename directories and files
- track what changed
- generate an operation log
- support rollback/undo

## 4. AI Planning Layer
Later, add an optional AI planning layer that can consume the scanned inventory and suggest a plan.

Important design rule:
- AI should propose a plan
- Fileforge should validate and display the plan
- the user should approve before apply

Potential AI-assisted behaviors:
- infer project folders from filenames/content hints
- suggest better category names
- cluster files by topic or use case
- propose cleanup of messy download folders
- explain *why* items are being grouped

## Development Phases

## Phase 1 — Solid CLI Engine
**Goal:** make the core file logic trustworthy.

### Milestones
- [x] Basic rename preview/apply
- [x] Messy fixture generator for tests
- [x] Integration-style sort test
- [ ] Recursive inventory scanner
- [ ] Structured inventory output (JSON/table)
- [ ] Preview plan model (no file moves)
- [ ] Config-driven sort rules
- [ ] Duplicate detection groundwork
- [ ] Undo/rollback log format

### Output
A reliable CLI engine that can scan, plan, and apply changes with tests.

## Phase 2 — Real Preview System
**Goal:** preview future structure without moving files.

### Milestones
- [ ] inventory model for full directory trees
- [ ] plan generator from rules + inventory
- [ ] preview command that shows proposed folder structure
- [ ] conflict detection and skip logic
- [ ] summary view: counts, file types, large files, empty folders
- [ ] export preview to JSON for GUI consumption

### Output
A fast preview system that feels like: "here is what your folder *would* look like if you applied this plan."

## Phase 3 — Smarter Rules and Safety
**Goal:** make Fileforge feel trustworthy enough for real user data.

### Milestones
- [ ] saved rule presets
- [ ] filter/include/exclude patterns
- [ ] duplicate detection with hashes
- [ ] rollback support
- [ ] operation history/logs
- [ ] large-folder performance testing
- [ ] Windows path edge-case coverage

## Phase 4 — Windows Desktop App Alpha
**Goal:** first usable GUI for Windows.

### Recommended path
Two realistic options:
- **Qt desktop app** with C++ core
- **C#/.NET desktop shell** calling into the core engine

Current instinct: keep the engine in C++, then choose the GUI path based on how much native Windows polish vs development speed we want.

### Milestones
- [ ] choose UI stack
- [ ] folder picker + drag/drop
- [ ] scan button
- [ ] inventory/summary view
- [ ] proposed plan preview tree
- [ ] apply button with confirmation
- [ ] progress UI and cancel support
- [ ] rollback UI

## Phase 5 — Windows Packaging
**Goal:** make it install like a normal app.

### Milestones
- [ ] Windows build pipeline
- [ ] app icon, metadata, versioning
- [ ] signed installer plan
- [ ] Inno Setup / WiX / NSIS packaging
- [ ] Start Menu entry + uninstall flow
- [ ] release artifacts on GitHub

### Output
A downloadable `.exe` installer for Windows users.

## Phase 6 — Optional AI Sorting
**Goal:** allow users to plug in an AI model without making the app dependent on one.

### Milestones
- [ ] AI provider abstraction
- [ ] local-model or API-model plug-in path
- [ ] prompt/input format from inventory model
- [ ] AI-generated plan validation
- [ ] explanation UI for AI decisions
- [ ] privacy/security controls
- [ ] offline/manual-only mode remains first-class

## Near-Term Plan (What we should work on next)

### Immediate next build steps
1. **Recursive inventory scanner**  
   Build the scanner that produces the internal inventory/data-table model.

2. **Plan preview format**  
   Represent proposed moves/renames as data, not as immediate filesystem actions.

3. **Preview command**  
   Add a command that scans a messy tree and prints a proposed future structure.

4. **Configurable rules**  
   Move beyond hard-coded extension buckets.

5. **Windows app shell decision**  
   Once the core preview/apply loop feels real, choose Qt vs C#/.NET for the GUI.

## Suggested UX Flow

Long-term ideal Windows flow:
1. User selects a folder
2. Fileforge scans everything recursively
3. Fileforge builds an inventory/understanding of contents
4. Fileforge generates a proposed organization plan
5. User reviews the new structure in preview
6. User tweaks rules if needed
7. User clicks Apply
8. Fileforge executes operations and stores rollback history

## Decision Notes

- The preview system should be **plan-based**, not copy/move-based.
- The inventory model is the foundation for both rules and future AI.
- AI should sit on top of the inventory and planning system, not replace it.
- Windows installer/distribution should come after the engine and preview loop feel trustworthy.
