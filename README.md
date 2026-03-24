# Fileforge

A practical desktop file operations tool for power users and small businesses who need to organize messy folders without risky one-off scripts. Batch renames with preview, rule-based sorting, duplicate detection, and safe bulk operations.

## Current MVP (v0.1)

A CLI tool that previews and applies batch renames in a directory.

**Rename rule:** lowercase the filename stem, replace spaces and underscores with dashes, preserve the file extension.

```
$ fileforge rename ~/messy-folder
[DRY-RUN] Batch rename in: /home/you/messy-folder

  My Document.pdf  ->  my-document.pdf
  Photo_2024.JPG   ->  photo-2024.JPG

2 file(s) would be renamed. Run with --apply to execute.

$ fileforge rename ~/messy-folder --apply
[APPLY] Batch rename in: /home/you/messy-folder

  My Document.pdf  ->  my-document.pdf
  Photo_2024.JPG   ->  photo-2024.JPG

2 file(s) renamed.
```

## Build

Requires CMake 3.16+ and a C++17 compiler.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The binary is at `build/fileforge`.

## Product Direction

Fileforge is headed toward a **Windows-first desktop app** with an installable release, fast scan-based previews, safe apply/rollback behavior, and optional AI-assisted organization.

Near-term direction:
- recursive inventory scanning
- preview plans generated **without moving files first**
- configurable rule-based sorting
- duplicate detection and rollback groundwork
- eventual Windows GUI + installer
- optional AI model plug-in for advanced planning

See [ROADMAP.md](./ROADMAP.md) for the full build plan.

## License

TBD
