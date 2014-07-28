
chrome.runtime.onMessage.addListener(function (request, sender, sendResponse) {
    console.log(request);
    if (sender.tab) {
        if (request == "closeTab") {
            chrome.tabs.remove(sender.tab.id);
        }
        if (request == "focusTab") {
            chrome.tabs.update(sender.tab.id, { selected: true });
        }
    }
});