#!/bin/sh
# Valida VERSION, CHANGELOG.md y, si se indica, el tag de git.
# Uso: scripts/check_version.sh [tag]
set -eu

cd "$(dirname "$0")/.."

version=$(cat VERSION)
if ! echo "$version" | grep -Eq '^[0-9]+\.[0-9]+\.[0-9]+$'; then
  echo "VERSION no es SemVer (X.Y.Z): $version" >&2
  exit 1
fi

if ! grep -q "^## \[$version\]" CHANGELOG.md; then
  echo "CHANGELOG.md no tiene una sección ## [$version]" >&2
  exit 1
fi

if [ "$#" -ge 1 ]; then
  if [ "$1" != "v$version" ]; then
    echo "El tag $1 no coincide con v$version" >&2
    exit 1
  fi
fi

echo "versión $version correcta"
