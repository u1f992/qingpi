import tomllib
import zipfile
import os

with open("pyproject.toml", "rb") as f:
    data = tomllib.load(f)
    version = data["tool"]["poetry"]["version"]
    with zipfile.ZipFile(f"qingpictl_v{version}.zip", "w", zipfile.ZIP_DEFLATED) as zf:
        zf.write("dist/qingpictl.exe", os.path.basename("dist/qingpictl.exe"))
        zf.write("README.md", os.path.basename("README.md"))
