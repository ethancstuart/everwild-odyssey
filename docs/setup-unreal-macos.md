# Unreal Setup On macOS

Everwild Odyssey is an Unreal Engine C++ project. Use an installed local Unreal version if one is already available. If Unreal is not installed, install it before trying to build or open the project.

## Current Local Status

At plan creation time, the machine had:

- Xcode 26.3
- Apple clang 17.0.0
- No Unreal Engine install found in common macOS locations
- No Epic Games Launcher found in common macOS locations
- No Git LFS command found

## Setup Steps

1. Install Epic Games Launcher from Epic's official Unreal Engine installation flow.
2. Install the latest stable Unreal Engine 5.x version available in the launcher, unless the project has already been associated with a specific local version.
3. Open `EverwildOdyssey.uproject`.
4. Let Unreal generate project files when prompted.
5. Build the `EverwildOdysseyEditor` target.
6. Run `Scripts/check-unreal.sh` from the project root after installation.

## Git LFS

The repo includes `.gitattributes` rules for Unreal binary assets, but the current first pass should avoid committing binary `.uasset` and `.umap` files until Git LFS is installed and initialized.

## Local-Only Assets

Do not commit Marketplace or premium assets unless their license explicitly allows public redistribution. Put local-only content in:

- `Content/LocalOnly/`
- `Content/Marketplace/`
- `Plugins/Marketplace/`

These paths are ignored by git.
