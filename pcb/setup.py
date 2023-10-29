import dataclasses
import os
import shutil
import subprocess
import zipfile


@dataclasses.dataclass
class LCSCComponent:
    name: str
    id: str

    def download(self, dest: str):
        os.makedirs(os.path.join(dest, self.name))
        subprocess.run(
            [
                "easyeda2kicad",
                "--full",
                f"--lcsc_id={self.id}",
                f"--output={os.path.join(dest, self.name, self.name)}",
            ],
            check=True,
        )


@dataclasses.dataclass
class ZipComponentFile:
    source: str
    dest: str


def extract_to_dest(zip: zipfile.ZipFile, info: zipfile.ZipInfo, dest: str):
    dest_dir = os.path.dirname(dest)
    os.makedirs(dest_dir, exist_ok=True)

    with zip.open(info) as source_file, open(dest, "wb") as dest_file:
        shutil.copyfileobj(source_file, dest_file)


@dataclasses.dataclass
class ZipComponent:
    zip: str
    source: str
    name: str
    files: list[ZipComponentFile]

    def expand(self, dest: str):
        dest_dir = os.path.join(dest, self.name)
        os.makedirs(dest_dir)

        with zipfile.ZipFile(self.zip, "r") as zip:
            for info in zip.infolist():
                for file in self.files:
                    if info.filename == file.source:
                        dest_path = os.path.join(dest_dir, file.dest)
                        extract_to_dest(zip, info, dest_path)
                        print(f"{info.filename}\n    => {dest_path}")


LIB_DIR = "./lib"
LCSC_COMPONENTS = [
    LCSCComponent(name="AD8403ARUZ1-REEL", id="C652783"),
    LCSCComponent(name="DS4432U+T&R", id="C2679620"),
    LCSCComponent(name="RP2040", id="C2040"),
    LCSCComponent(name="W25Q16JVUXIQ", id="C2843335"),
    LCSCComponent(name="THD0515-24CL-SN", id="C283150"),
    LCSCComponent(name="AP2112K-3.3TRG1", id="C51118"),
    # Alt: LCSCComponent(name="NCP114BSN330T1G", id="C327507")
    LCSCComponent(name="TYPE-C-31-M-12", id="C165948"),
]
LOCAL_ZIP_REQUIRED = [
    ZipComponent(
        zip="LIB_G3VM-31WR_TR05_.zip",
        source="https://componentsearchengine.com/part-view/G3VM-31WR(TR05)/Omron%20Electronics",
        name="G3VM-31WR_TR05_",
        files=[
            ZipComponentFile(
                source="G3VM-31WR_TR05_/3D/G3VM-31WR_TR05_.stp",
                dest="G3VM-31WR_TR05_.stp",
            ),
            ZipComponentFile(
                source="G3VM-31WR_TR05_/KiCad/G3VM-31WR_TR05_.kicad_sym",
                dest="G3VM-31WR_TR05_.kicad_sym",
            ),
            ZipComponentFile(
                source="G3VM-31WR_TR05_/KiCad/G3VM31WRTR05.kicad_mod",
                dest="G3VM-31WR_TR05_.pretty/G3VM-31WR_TR05_.kicad_mod",
            ),
        ],
    ),
]


def main():
    for c in LOCAL_ZIP_REQUIRED:
        if not os.path.isfile(c.zip):
            print(
                f'Please download "{c.zip}" from {c.source} and place it in this directory.'
            )
            return 1

    if os.path.exists(LIB_DIR):
        shutil.rmtree(LIB_DIR)
    os.makedirs(LIB_DIR)

    for c in LCSC_COMPONENTS:
        print(c.name)
        c.download(LIB_DIR)
        print()

    for c in LOCAL_ZIP_REQUIRED:
        print(c.name)
        c.expand(LIB_DIR)
        print()

    return 0


if __name__ == "__main__":
    exit(main())
