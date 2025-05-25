const port = browser.runtime.connectNative("tab_switcher_host");

port.onMessage.addListener((message) => {
  console.log("Received message:", message);

  if (message.action === "switch-left" || message.action === "switch-right") {
    browser.tabs.query({ currentWindow: true }).then((tabs) => {
      browser.tabs.query({ active: true, currentWindow: true }).then((activeTabs) => {
        const activeIndex = activeTabs[0].index;
        const direction = message.action === "switch-left" ? -1 : 1;
        let nextIndex = activeIndex + direction;

        if (nextIndex < 0) {
          nextIndex = tabs.length - 1;
        } else if (nextIndex >= tabs.length) {
          nextIndex = 0;
        }

        const nextTab = tabs.find(tab => tab.index === nextIndex);
        if (nextTab) {
          browser.tabs.update(nextTab.id, { active: true });
        }
      });
    });
  }
});

port.onDisconnect.addListener(() => {
  console.error("Disconnected from native app");
});
