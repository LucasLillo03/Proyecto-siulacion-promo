const selector = document.getElementById('file-selector');
const status = document.getElementById('status');
const plotDiv = document.getElementById('plot');
const clearBtn = document.getElementById('clear-btn');

const activeTraces = {};
const colorPalette = [
  '#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd',
  '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf',
];

async function init() {
  const result = await window.api.getCSVList();
  if (!result.success) {
    status.textContent = 'Error al leer outputs/: ' + result.error;
    return;
  }

  if (result.data.length === 0) {
    status.textContent = 'No se encontraron archivos .csv en outputs/';
    return;
  }

  result.data.forEach((f, i) => {
    const item = document.createElement('div');
    item.className = 'file-item';
    item.dataset.file = f;
    item.innerHTML = `
      <span class="file-color" style="background:${colorPalette[i % colorPalette.length]}"></span>
      <span class="file-name">${f}</span>
      <span class="file-check"></span>
    `;
    item.addEventListener('click', () => toggleFile(f));
    selector.appendChild(item);
  });

  status.textContent = 'Selecciona uno o mas archivos para graficar';
  clearBtn.addEventListener('click', clearAll);
}

async function toggleFile(filename) {
  const item = document.querySelector(`.file-item[data-file="${filename}"]`);

  if (activeTraces[filename]) {
    delete activeTraces[filename];
    item.classList.remove('active');
    updatePlot();
    return;
  }

  status.textContent = 'Leyendo ' + filename + '...';
  const result = await window.api.readCSV(filename);

  if (!result.success) {
    status.textContent = 'Error al leer ' + filename + ': ' + result.error;
    return;
  }

  const parsed = Papa.parse(result.data, { header: false }).data;
  const rows = parsed.filter(r => r.length >= 2 && r[0] !== '' && r[1] !== '');
  const x = rows.map(r => parseFloat(r[0]));
  const y = rows.map(r => parseFloat(r[1]));

  const colorIdx = [...selector.children].indexOf(item) % colorPalette.length;

  activeTraces[filename] = {
    x,
    y,
    type: 'scatter',
    mode: 'lines+markers',
    name: filename.replace('.csv', ''),
    marker: { size: 4, color: colorPalette[colorIdx] },
    line: { width: 2, color: colorPalette[colorIdx] },
  };

  item.classList.add('active');
  status.textContent = '';
  updatePlot();
}

function updatePlot() {
  const traces = Object.values(activeTraces);
  const count = traces.length;

  const layout = {
    title: count === 0 ? 'Selecciona archivos para graficar' : count + ' archivo(s) seleccionado(s)',
    xaxis: { title: 'Tiempo (s)' },
    yaxis: { title: 'Valor' },
    margin: { l: 60, r: 30, t: 50, b: 60 },
    paper_bgcolor: '#f8f9fa',
    plot_bgcolor: '#f8f9fa',
    showlegend: count > 1,
  };

  Plotly.react(plotDiv, traces, layout, { responsive: true });
  status.textContent = '';
}

function clearAll() {
  for (const file in activeTraces) {
    const item = document.querySelector(`.file-item[data-file="${file}"]`);
    if (item) item.classList.remove('active');
  }
  for (const key in activeTraces) delete activeTraces[key];
  updatePlot();
}

init();
