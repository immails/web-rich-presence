I'm sorry for the ass code, I'm kinda new in the c++ world.

> [!WARNING]
> Currently only supports Windows

- uses Discord Social SDK
- uses https://github.com/crowcpp/crow
- uses https://github.com/nlohmann/json

### Packaging
The release archive contains:
- WebRichPresenceW.exe — the software itself;
- WebRichPresence.exe — a verbose version of the software that launches with a terminal for troubleshooting purposes;
- discord_social_sdk.dll — a vital component of the software.

### Installation
1. Download the latest release archive from the releases.
2. Extract it somewhere. The software is portable (as long as it was launched once in a new directory if was moved).
3. Launch **WebRichPresence*W*.exe**.

### Usage
0. First of all, the software is **not required to be launched manually**. It is launched by a browser extension when it is needed and closes itself when it's not.
1. Install a compatible browser extension.
2. Open a page that the extension can work with.
3. Accept browser's **webrichpresence://** launching prompt (make sure to **tick the checkbox** to make it work seamlessly).
