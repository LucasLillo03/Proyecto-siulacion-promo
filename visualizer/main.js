const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const fs = require('fs');

const OUTPUTS_DIR = path.resolve(__dirname, '..', 'outputs');

function createWindow() {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    title: 'PowerDEVS Visualizer',
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false,
    },
  });

  win.loadFile(path.join(__dirname, 'src', 'index.html'));
}

ipcMain.handle('get-csv-list', async () => {
  try {
    const files = fs.readdirSync(OUTPUTS_DIR).filter(f => f.endsWith('.csv'));
    return { success: true, data: files };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

ipcMain.handle('read-csv', async (event, filename) => {
  try {
    const filePath = path.join(OUTPUTS_DIR, path.basename(filename));
    const content = fs.readFileSync(filePath, 'utf-8');
    return { success: true, data: content };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});
