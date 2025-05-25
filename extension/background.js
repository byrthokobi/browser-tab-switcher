function connectNative() {
  const port = browser.runtime.connectNative("tab_switcher_host");

  port.onMessage.addListener((message) => {
    console.log("Received message from native app:", message);
    if (message.action === "switch-left") {
      switchTab(-1);
    } else if (message.action === "switch-right") {
      switchTab(1);
    }
  });

  port.onDisconnect.addListener(() => {
    console.error("Disconnected from native app");
  });
}

function switchTab(direction) {
  browser.tabs.query({ currentWindow: true }).then((tabs) => {
    browser.tabs.query({ active: true, currentWindow: true }).then(([activeTab]) => {
      const activeIndex = activeTab.index;
      const newIndex = (activeIndex + direction + tabs.length) % tabs.length;
      browser.tabs.update(tabs[newIndex].id, { active: true });
    });
  });
}

connectNative();
