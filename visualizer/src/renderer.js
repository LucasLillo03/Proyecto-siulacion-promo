const selector = document.getElementById('file-selector');
const status = document.getElementById('status');
const plotDiv = document.getElementById('plot');
const clearBtn = document.getElementById('clear-btn');
const showAlarmsCheck = document.getElementById('show-alarms');
const showStateBandsCheck = document.getElementById('show-state-bands');

const activeTraces = {};
const colorPalette = [
  '#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd',
  '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf',
];

const stateNames = ['normal', 'desviado', 'critico'];
const stateBandColors = [
  'rgba(0, 200, 83, 0.08)',
  'rgba(255, 193, 7, 0.15)',
  'rgba(255, 50, 50, 0.12)',
];

let alarmIntervals = [];

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
  showAlarmsCheck.addEventListener('change', onAlarmsToggle);
  showStateBandsCheck.addEventListener('change', onStateBandsToggle);

  await loadAlarmIntervals();

  if (result.data.includes('estados.csv')) {
    await toggleFile('estados.csv');
  }
}

async function loadAlarmIntervals() {
  const result = await window.api.readCSV('alarmas.csv');
  if (!result.success) {
    alarmIntervals = [];
    return;
  }

  const parsed = Papa.parse(result.data, { header: false }).data;
  const rows = parsed.filter(r => r.length >= 2 && r[0] !== '' && r[1] !== '');
  alarmIntervals = findCriticalIntervals(rows);
}

function findCriticalIntervals(rows) {
  const critical = rows.filter(r => parseInt(r[1]) === 1);
  if (critical.length === 0) return [];

  const intervals = [];
  let start = parseFloat(critical[0][0]);

  for (let i = 1; i < critical.length; i++) {
    const gap = parseFloat(critical[i][0]) - parseFloat(critical[i - 1][0]);
    if (gap > 1.5) {
      intervals.push({ start, end: parseFloat(critical[i - 1][0]) });
      start = parseFloat(critical[i][0]);
    }
  }
  intervals.push({ start, end: parseFloat(critical[critical.length - 1][0]) });

  return intervals;
}

function getAlarmShapes() {
  return alarmIntervals.map(({ start, end }) => ({
    type: 'rect',
    x0: start,
    x1: end,
    yref: 'paper',
    y0: 0,
    y1: 1,
    fillcolor: 'rgba(255, 50, 50, 0.15)',
    line: { width: 0 },
  }));
}

function onAlarmsToggle() {
  updatePlot();
}

function parseEstadoCSV(rows) {
  const x = [], y = [], intervals = [];
  for (const r of rows) {
    const t = parseFloat(r[0]);
    const s = r[1].trim().toLowerCase();
    const val = stateNames.indexOf(s) * 100;
    if (val === -1) continue;
    x.push(t);
    y.push(val);
  }
  for (let i = 0; i < x.length - 1; i++) {
    intervals.push({ start: x[i], end: x[i + 1], state: y[i] / 100 });
  }
  return { x, y, intervals };
}

function getEstadoShapes(intervals) {
  return intervals.map(({ start, end, state }) => ({
    type: 'rect',
    x0: start,
    x1: end,
    yref: 'paper',
    y0: 0,
    y1: 1,
    fillcolor: stateBandColors[state] || 'rgba(0,0,0,0.05)',
    line: { width: 0 },
  }));
}

function onStateBandsToggle() {
  updatePlot();
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
  const colorIdx = [...selector.children].indexOf(item) % colorPalette.length;

  if (filename === 'estados.csv') {
    const estadoData = parseEstadoCSV(rows);
    activeTraces[filename] = {
      x: estadoData.x,
      y: estadoData.y,
      intervals: estadoData.intervals,
      type: 'scatter',
      mode: 'lines',
      name: 'estados',
      line: { shape: 'hv', width: 2, color: colorPalette[colorIdx] },
    };
  } else {
    const x = rows.map(r => parseFloat(r[0]));
    const y = rows.map(r => parseFloat(r[1]));
    activeTraces[filename] = {
      x,
      y,
      type: 'scatter',
      mode: filename.includes('.line') ? 'lines+markers' : 'markers',
      name: filename.replace('.csv', ''),
      marker: { size: 4, color: colorPalette[colorIdx] },
      line: { width: 2, color: colorPalette[colorIdx] },
    };
  }

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

  const shapes = [];

  if (showStateBandsCheck.checked) {
    for (const key in activeTraces) {
      if (activeTraces[key].intervals) {
        shapes.push(...getEstadoShapes(activeTraces[key].intervals));
      }
    }
  }

  if (showAlarmsCheck.checked && alarmIntervals.length > 0) {
    shapes.push(...getAlarmShapes());
  }

  if (shapes.length > 0) {
    layout.shapes = shapes;
  }

  const hasEstados = Object.values(activeTraces).some(t => t.intervals);
  if (hasEstados) {
    layout.yaxis = {
      ...layout.yaxis,
      tickvals: [0, 100, 200],
      ticktext: ['normal', 'desviado', 'critico'],
    };
  }

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
