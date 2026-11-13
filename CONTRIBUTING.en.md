# Contributing to UnLua (bytepine extraction)

[中文](CONTRIBUTING.md) | English

This repository is an unofficial extraction of [Tencent/UnLua](https://github.com/Tencent/UnLua)'s `Plugins/UnLua`, based on v2.3.6. It is **not** Tencent's official repository. See [LICENSE.TXT](LICENSE.TXT) for upstream notices; retain Tencent copyright headers when modifying upstream source files.

## Documentation

All maintained documentation must provide complete Chinese and English versions, in separate Chinese and English files with reciprocal links at the top. Update both languages together, including README files, contribution and technical guides, and all changelog entries. Keep commands, paths, API names, and version numbers consistent; update generated documentation through its source or generator. Retain upstream copyright and license text unchanged.

## Commit messages

Chinese Conventional Commits: `<type>(<scope>): <subject>`.

- `type`: `feat` / `fix` / `perf` / `refactor` / `docs` / `chore`.
- `scope`: `plugin` / `compat` / `lua` / `docs`; optional.
- **Commit CHANGELOG updates together with the code change**, never separately.

## CHANGELOG

For actual functionality changes, append to `[Unreleased]` in [CHANGELOG.md](CHANGELOG.md) and [CHANGELOG.en.md](CHANGELOG.en.md). Pure formatting / lint changes do not need an entry. Record only this repository's changes relative to upstream v2.3.6; see the [upstream CHANGELOG](https://github.com/Tencent/UnLua/blob/master/CHANGELOG.md) for official history.

### Merge categories (required)

Within each version section, including `[Unreleased]`, each heading may appear **only once**: `### Added` / `### Changed` / `### Fixed` / `### Deprecated` / `### Removed` / `### Security`.

- When a heading already exists, merge new entries into that subsection, appending them to its list; do not create another identical `###` heading.
- If duplicate subsection headings already exist, merge them first: move later entries into the first matching subsection and remove the later headings.
- Categorize by meaning: `feat`→Added; `fix`→Fixed; `perf`/`refactor`/`chore`/`docs` with behavior changes→Changed. Each bullet belongs to one subsection only.

### Merge content (required)

Before appending, scan existing entries in the same subsection. Do not split the **same topic / module / fix** into multiple bullets.

| Situation | Action |
|-----------|--------|
| Duplicate or nearly synonymous wording | Remove redundancy; keep the most complete entry |
| Further changes to the same module | Rewrite the existing bullet, combining details with commas or semicolons |
| Multiple fixes with the same root cause | Merge into one entry that states the final behavior and key identifiers |
| Different wording with identical facts | Treat as duplicates and merge |

Do not accumulate repeated bullets just to record each commit. Readers should see a deduplicated set of capabilities/fixes, not fragments ordered by commit time.

## Versions

Daily commits update **only** `CHANGELOG [Unreleased]`; do **not** change `UnLua.uplugin`'s `VersionName` or create tags.

At release time, archive `[Unreleased]` as `[X.Y.Z] - YYYY-MM-DD` and update `VersionName`. New features increment minor; bug fixes increment patch; breaking changes increment major. Versions describe **this repository's changes relative to upstream v2.3.6**. Do not attribute official upstream commits after 2.3.6 to this repository's versions.
