# Heroic MMO Asset Manifest

Date: 2026-06-15

## Repository Policy

The public repo contains source code, docs, scripts, and lightweight original assets only. Marketplace, premium, Vault cache, and local-only binary assets stay outside git unless their license and file size are approved for public redistribution.

## Local Asset Roots

- `Content/LocalOnly/`
- `Content/Marketplace/`
- `Plugins/Marketplace/`
- Unreal Engine content under `/Users/Shared/Epic Games/UE_5.7/Engine/Content/`

## Required Showcase Roles

Required roles are defined in `FEOAssetRoleCatalog::BuildDefaultRoles`. Showcase Mode fails visual acceptance when any required role is missing and has no explicit proxy resolution.

## Current Asset Source Preference

1. Installed local project assets.
2. Unreal Engine sample/plugin content already installed.
3. License-safe free assets with user-approved network/install actions.
4. Explicit debug proxies with visible diagnostics.

## Current Known Gap

The initial catalog points to `Content/LocalOnly` paths for final-looking hero, enemy, environment, VFX, and UI roles. If those assets are absent, the resolver reports proxy or missing state and the visual verification doc records the gap.
