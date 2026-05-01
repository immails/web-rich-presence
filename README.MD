I'm sorry for the ass code, I'm kinda new in the c++ world.

> [!WARNING]
> Currently only supports Windows

### User Usage
0. First of all, the software is **not required to be launched manually**. It is launched by a browser extension when it is needed and closes itself when it's not.
1. Install a compatible browser extension.
2. Open a page that the extension can work with.
3. Accept browser's **webrichpresence://** launching prompt (make sure to **tick the checkbox** to make it work seamlessly).

### Compatible Extensions:
- [SoundCloud Rich Presence](https://addons.mozilla.org/en-US/firefox/addon/soundcloud-rich-presence/) \
![SoundCloud Rich Presence Example](https://github.com/immails/web-rich-presence/blob/main/example.png?raw=true)

### Packaging
The release archive contains:
- WebRichPresenceW.exe — the software itself;
- WebRichPresence.exe — a verbose version of the software that launches with a terminal for troubleshooting purposes;
- discord_partner_sdk.dll — a vital component of the software.

### Installation
1. Download [the latest release archive](https://github.com/immails/web-rich-presence/releases/latest/download/release.zip) from the releases.
2. Extract it somewhere. The software is portable (as long as it was launched once in a new directory if was moved).
3. Launch **WebRichPresenceW.exe**.

### Developer Usage
1. Launch the software from JavaScript by opening the `webrichpresence://<identifier>/<port>"` URI, e.g. `window.location.href = "webrichpresence://<identifier>/<port>"`;
2. Connect to the software via [WebSocket](https://developer.mozilla.org/en-US/docs/Web/API/WebSocket): `ws://localhost:<port>/<identifier>`;
3. Start sending commands. Types are available at [webrichpresence.types.ts](https://github.com/immails/web-rich-presence/blob/main/webrichpresence.types.ts). 

### About
- uses Discord Social SDK
- uses https://github.com/crowcpp/crow
- uses https://github.com/nlohmann/json
