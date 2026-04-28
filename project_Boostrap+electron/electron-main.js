const { app, BrowserWindow, Menu, shell } = require('electron');
const path = require('path');

function createWindow() {
  const win = new BrowserWindow({
    width: 1360,
    height: 860,
    minWidth: 1100,
    minHeight: 720,
    title: 'UniBudget Pro Desktop',
    backgroundColor: '#0f172a',
    webPreferences: {
      contextIsolation: true,
      nodeIntegration: false
    }
  });

  win.loadFile(path.join(__dirname, 'index.html'));
  win.webContents.setWindowOpenHandler(({ url }) => {
    shell.openExternal(url);
    return { action: 'deny' };
  });
}

const template = [
  {
    label: 'Soubor',
    submenu: [
      { role: 'reload', label: 'Obnovit' },
      { role: 'toggleDevTools', label: 'Developer tools' },
      { type: 'separator' },
      { role: 'quit', label: 'Ukončit' }
    ]
  },
  {
    label: 'Zobrazení',
    submenu: [
      { role: 'zoomIn', label: 'Přiblížit' },
      { role: 'zoomOut', label: 'Oddálit' },
      { role: 'resetZoom', label: 'Výchozí zoom' },
      { role: 'togglefullscreen', label: 'Celá obrazovka' }
    ]
  }
];

app.whenReady().then(() => {
  Menu.setApplicationMenu(Menu.buildFromTemplate(template));
  createWindow();
  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});
