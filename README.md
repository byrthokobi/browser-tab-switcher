<h1>Tab Switcher Extension - Documentation</h1>

<h2>📌 Overview </h2>

<p>This Firefox extension allows users to switch browser tabs quickly by moving the mouse to the left or right edge of the screen.<br> 
It uses native messaging (C++ backend) for smooth performance and CSS animations for a polished user experience.</p>


<h2>✨ Features</h2>

✅ Edge-Triggered Tab Switching

    Move mouse to the left edge → Switch to the previous tab.
    Move mouse to the right edge → Switch to the next tab.

✅ Smooth Slide Animations
    
    Fade-in and slide effects when switching tabs.

✅ Loop Navigation

    If on the first tab, switching left goes to the last tab.

    If on the last tab, switching right goes to the first tab.

✅ Native Performance

    Uses a C++ backend (mouse_tracker.exe) for low-latency edge detection.

⚙️ Installation
1. Install the Firefox Extension
For Testing (Temporary Install)

    Go to about:debugging#/runtime/this-firefox.

    Click "Load Temporary Add-on" → Select manifest.json.

For Permanent Install (Signed .XPI)

    Package the extension into a .zip and rename to .xpi.

    Drag & drop the .xpi into Firefox (about:addons).

2. Set Up the Native App (Required)

    Compile mouse_tracker.cpp into mouse_tracker.exe:
    bash: g++ mouse_tracker.cpp -o mouse_tracker.exe -static-libgcc -static-libstdc++

Register the Native Host:

    Place tab_switcher_host.json in:

    %APPDATA%\Mozilla\NativeMessagingHosts\  

    Ensure the "path" in the JSON points to mouse_tracker.exe.

For System-Wide Install (Admin Required):

    Place tab_switcher_host.json in: C:\Program Files\Mozilla Native Messaging Hosts\

Add a registry key:

        HKEY_LOCAL_MACHINE\SOFTWARE\Mozilla\NativeMessagingHosts\tab_switcher_host

        → Set value to the JSON file path.

🔧 How It Works
📂 File Structure

browser-tab-switcher/
├── extension/
│   ├── manifest.json
│   ├── background.js
├── native/
│   ├── mouse_tracker.cpp
│   ├── tab_switcher_host.json

🖱️ Native App (mouse_tracker.exe)

    Polls mouse position every 20ms.

    Sends {"action":"switch-left"} or {"action":"switch-right"} to Firefox when the mouse hits the screen edge.

🌐 Extension Logic (background.js)

    Listens for messages from mouse_tracker.exe.
    Finds the current tab and next tab index.
    Uses browser.tabs.update() to switch tabs.


🚀 Customization
⚡ Faster/Slower Switching

    Modify sleep_for in mouse_tracker.cpp:
    cpp

    this_thread::sleep_for(chrono::milliseconds(20)); // Lower = faster response


🔒 Restrict to Specific Websites

Update manifest.json:
json

"content_scripts": [{
  "matches": ["*://*.example.com/*"],
  "js": ["content.js"]
}]

