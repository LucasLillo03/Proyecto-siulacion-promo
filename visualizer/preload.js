const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
  getCSVList: () => ipcRenderer.invoke('get-csv-list'),
  readCSV: (filename) => ipcRenderer.invoke('read-csv', filename),
});
