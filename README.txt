#####
README file

The toolchain Desktop Qt 5.12.4 MinGW 64-bit

The command panel shows the following commands:
RESET_CMD per il reset del target
READ_BOOT_VERSION per leggere la versione del Bootloader
READ_FW_VERSION per leggere la versione del Bootloader
READ_FLASH per lettura di “N” bytes dalla flash (PFM -> Program Flash Memory)
WRITE_FLASH per scrittura di “N” blocchi da M (M=2 or 256 -> page write operation) bytes dalla flash
ERASE_FLASH per la cancellazione di “N” blocchi da 256 bytes (come da Data-Sheet)
START_FW_UPGRADE, per l’avvio della procedura di FW (sia per Bootloader sia per Applicazione-FW)

READ_Revision_ID per la lettura e visualizzazione nel Tool-SW della “Revision ID” del target
READ_Device_ID per la lettura e visualizzazione nel Tool-SW della “Device ID” del target

READ_FW_VERSION
READ_BOOT_VERSION.
