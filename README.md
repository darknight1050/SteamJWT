# SteamJWT
Decrypt and encrypt the Steam login JWT token.

Steam stores the local JWT tokens at `%appdata%/../Local/Steam/local.vdf` and protects them with `CryptProtectData` from `Crypt32.dll` ([Documentation](https://learn.microsoft.com/en-us/windows/win32/api/dpapi/nf-dpapi-cryptprotectdata)). \
This API uses unique system identifiers for de/encryption.

The additional entropy is the steam account username.

This code allows de/encryption of the token by calling the functions form  `Crypt32.dll`.

Some installations might have a `RememberedMachineID` token in the `Steam/config/config.vdf`
 file. \
It is unprotected but still needed for the installations that have it.
