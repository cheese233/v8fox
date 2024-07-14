import { Buffer } from "buffer/";
class PageContent {
    #port: browser.runtime.Port;
    constructor(tab: browser.tabs.Tab) {
        this.#port = browser.runtime.connectNative("v8fox_server");
        console.log(this.#port);
        this.#port.onMessage.addListener((response) => {
            console.log("Received: " + response);
        });
    }
    remove() {
        this.#port.disconnect();
    }
}
let pageContents: PageContent[] = [];
browser.tabs.onCreated.addListener(async (tab) => {
    console.log("new tab", tab);
    if (!tab.id) {
        return;
    }
    pageContents[tab.id] = new PageContent(tab);
});
browser.tabs.onRemoved.addListener((tabId) => {
    if (!pageContents[tabId]) return;
    pageContents[tabId].remove();
    delete pageContents[tabId];
});
browser.tabs.onUpdated.addListener(async (tabId, changeInfo, tab) => {});

browser.webRequest.onBeforeRequest.addListener(
    async (details) => {
        console.log("load script ", details.url, "from", details.tabId);
        let fakeURL =
            "data:text/javascript," +
            Buffer.from(
                await (await fetch(details.url)).arrayBuffer()
            ).toString("base64");
        return {
            redirectUrl: fakeURL,
        };
    },
    {
        urls: ["<all_urls>"],
        types: ["script"],
    },
    ["blocking"]
);
browser.webRequest.onBeforeRequest.addListener(
    (details) => {
        console.log("load page ", details.url, "from", details.tabId);
    },
    {
        urls: ["<all_urls>"],
        types: ["main_frame"],
    }
);
