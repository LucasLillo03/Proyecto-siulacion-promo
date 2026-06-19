const select = document.getElementById('csv-select');
const status = document.getElementById('status');
const plotDiv = document.getElementById('plot');

let allTraces = {};

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

  result.data.forEach(f => {
    const opt = document.createElement('option');
    opt.value = f;
    opt.textContent = f;
    select.appendChild(opt);
  });

  status.textContent = 'Selecciona un archivo CSV para graficar';
  select.addEventListener('change', onSelectChange);
}

async function onSelectChange() {
  const file = select.value;
  if (!file) return;

  status.textContent = 'Leyendo ' + file + '...';
  const result = await window.api.readCSV(file);

  if (!result.success) {
    status.textContent = 'Error al leer ' + file + ': ' + result.error;
    return;
  }

  const parsed = Papa.parse(result.data, { header: false }).data;
  const rows = parsed.filter(r => r.length >= 2 && r[0] !== '' && r[1] !== '');
  const x = rows.map(r => parseFloat(r[0]));
  const y = rows.map(r => parseFloat(r[1]));

  const label = file.replace('.csv', '');

  const trace = {
    x,
    y,
    type: 'scatter',
    mode: 'lines+markers',
    name: label,
    marker: { size: 4 },
    line: { width: 2 },
  };

  const layout = {
    title: label,
    xaxis: { title: 'Tiempo (s)' },
    yaxis: { title: 'Valor' },
    margin: { l: 60, r: 30, t: 50, b: 60 },
    paper_bgcolor: '#f8f9fa',
    plot_bgcolor: '#f8f9fa',
  };

  Plotly.newPlot(plotDiv, [trace], layout, { responsive: true });
  status.textContent = '';
}

init();
