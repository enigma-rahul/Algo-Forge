/**
 * ALGO FORGE — Frontend Playback & Visualization Engine
 * 
 * Communicates with the C++ HTTP Engine via JSON API:
 *   - GET  /api/health
 *   - POST /api/visualize
 * 
 * Replays typed C++ events step-by-step (Array bars, Graph nodes/edges, Grid, Tree).
 * JavaScript DOES NOT implement algorithm logic.
 */

// ============================================================================
// 1. ALGORITHM CATALOG & METADATA
// ============================================================================

const ALGORITHMS = {
  // Sorting (Array bars)
  bubble_sort: {
    name: 'Bubble Sort',
    category: 'sorting',
    family: 'array',
    time: 'O(n²)',
    space: 'O(1)',
    description: 'Repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order.',
    defaultInput: { values: [29, 10, 14, 37, 13, 2, 45, 18] },
    presets: {
      'Default': [29, 10, 14, 37, 13, 2, 45, 18],
      'Reversed': [50, 42, 35, 28, 20, 15, 9, 3],
      'Nearly Sorted': [5, 10, 15, 25, 20, 30, 35, 40],
      'Random': () => generateRandomArray(8, 5, 60)
    }
  },
  selection_sort: {
    name: 'Selection Sort',
    category: 'sorting',
    family: 'array',
    time: 'O(n²)',
    space: 'O(1)',
    description: 'Finds the minimum element from the unsorted part and places it at the beginning, repeating for all positions.',
    defaultInput: { values: [34, 12, 56, 7, 23, 89, 45, 19] },
    presets: {
      'Default': [34, 12, 56, 7, 23, 89, 45, 19],
      'Reversed': [60, 48, 36, 24, 18, 12, 6],
      'Duplicates': [15, 8, 22, 8, 15, 30, 8, 22],
      'Random': () => generateRandomArray(8, 5, 70)
    }
  },
  insertion_sort: {
    name: 'Insertion Sort',
    category: 'sorting',
    family: 'array',
    time: 'O(n²)',
    space: 'O(1)',
    description: 'Builds the final sorted array one item at a time by repeatedly shifting larger elements right and inserting the key.',
    defaultInput: { values: [22, 11, 44, 33, 55, 8, 66, 15] },
    presets: {
      'Default': [22, 11, 44, 33, 55, 8, 66, 15],
      'Nearly Sorted': [4, 8, 12, 20, 16, 24, 28],
      'Few Unique': [10, 20, 10, 30, 20, 10, 30],
      'Random': () => generateRandomArray(8, 5, 70)
    }
  },
  merge_sort: {
    name: 'Merge Sort',
    category: 'sorting',
    family: 'array',
    time: 'O(n log n)',
    space: 'O(n)',
    description: 'Divide-and-conquer algorithm that recursively splits the array into halves, sorts them, and merges the sorted halves.',
    defaultInput: { values: [38, 27, 43, 3, 9, 82, 10, 19] },
    presets: {
      'Default': [38, 27, 43, 3, 9, 82, 10, 19],
      'Reversed': [70, 60, 50, 40, 30, 20, 10],
      'Random': () => generateRandomArray(8, 5, 80)
    }
  },
  quick_sort: {
    name: 'Quick Sort',
    category: 'sorting',
    family: 'array',
    time: 'O(n log n)',
    space: 'O(log n)',
    description: 'Picks a pivot element, partitions the array so smaller elements precede it and larger follow it, then recurses.',
    defaultInput: { values: [40, 20, 70, 10, 90, 30, 60, 50] },
    presets: {
      'Default': [40, 20, 70, 10, 90, 30, 60, 50],
      'Reversed': [80, 70, 60, 50, 40, 30, 20, 10],
      'Random': () => generateRandomArray(8, 5, 80)
    }
  },

  // Searching (Array bars)
  linear_search: {
    name: 'Linear Search',
    category: 'searching',
    family: 'search_array',
    time: 'O(n)',
    space: 'O(1)',
    description: 'Sequentially checks each element of the list until a match is found or the entire list has been searched.',
    defaultInput: { values: [15, 42, 8, 93, 27, 61, 34, 76], target: 27 },
    presets: {
      'Present': { values: [15, 42, 8, 93, 27, 61, 34, 76], target: 27 },
      'At Start': { values: [99, 12, 45, 67, 33, 21], target: 99 },
      'Not Present': { values: [14, 28, 35, 42, 56, 70], target: 100 }
    }
  },
  binary_search: {
    name: 'Binary Search',
    category: 'searching',
    family: 'search_array',
    time: 'O(log n)',
    space: 'O(1)',
    description: 'Efficiently searches a sorted array by repeatedly dividing the search interval in half. Rejects unsorted arrays.',
    defaultInput: { values: [6, 14, 25, 33, 47, 59, 72, 88], target: 47 },
    presets: {
      'Target 47': { values: [6, 14, 25, 33, 47, 59, 72, 88], target: 47 },
      'Target 6': { values: [6, 14, 25, 33, 47, 59, 72, 88], target: 6 },
      'Missing 50': { values: [10, 20, 30, 40, 60, 70, 80, 90], target: 50 },
      'Unsorted (Test Error)': { values: [10, 30, 20, 40, 50], target: 30 }
    }
  },

  // Graph Traversals & Shortest Path
  bfs: {
    name: 'Breadth-First Search (BFS)',
    category: 'graph',
    family: 'graph',
    time: 'O(V + E)',
    space: 'O(V)',
    description: 'Explores graph vertices layer by layer using a FIFO queue, visiting all immediate neighbors before moving deeper.',
    defaultInput: {
      nodes: [0, 1, 2, 3, 4, 5],
      edges: [[0, 1], [0, 2], [1, 3], [1, 4], [2, 4], [3, 5], [4, 5]],
      start: 0
    },
    presets: {
      'Sample Tree-like': {
        nodes: [0, 1, 2, 3, 4, 5],
        edges: [[0, 1], [0, 2], [1, 3], [1, 4], [2, 4], [3, 5], [4, 5]],
        start: 0
      },
      'Star Graph': {
        nodes: [0, 1, 2, 3, 4],
        edges: [[0, 1], [0, 2], [0, 3], [0, 4]],
        start: 0
      }
    }
  },
  dfs: {
    name: 'Depth-First Search (DFS)',
    category: 'graph',
    family: 'graph',
    time: 'O(V + E)',
    space: 'O(V)',
    description: 'Explores as far as possible along each branch before backtracking, using recursion or an explicit LIFO call stack.',
    defaultInput: {
      nodes: [0, 1, 2, 3, 4, 5],
      edges: [[0, 1], [0, 2], [1, 3], [2, 4], [3, 5], [4, 5]],
      start: 0
    },
    presets: {
      'Linear Depth': {
        nodes: [0, 1, 2, 3, 4, 5],
        edges: [[0, 1], [0, 2], [1, 3], [2, 4], [3, 5], [4, 5]],
        start: 0
      },
      'Cyclic Mesh': {
        nodes: [0, 1, 2, 3, 4],
        edges: [[0, 1], [1, 2], [2, 3], [3, 4], [4, 0], [1, 3]],
        start: 0
      }
    }
  },
  dijkstra: {
    name: "Dijkstra's Shortest Path",
    category: 'graph',
    family: 'weighted_graph',
    time: 'O((V + E) log V)',
    space: 'O(V)',
    description: 'Finds the shortest paths between nodes in a graph with non-negative edge weights using a priority queue.',
    defaultInput: {
      nodes: [0, 1, 2, 3, 4],
      edges: [
        [0, 1, 4],
        [0, 2, 2],
        [1, 2, 1],
        [1, 3, 5],
        [2, 3, 8],
        [2, 4, 10],
        [3, 4, 2]
      ],
      start: 0,
      target: 4
    },
    presets: {
      '5-Node Network': {
        nodes: [0, 1, 2, 3, 4],
        edges: [[0, 1, 4], [0, 2, 2], [1, 2, 1], [1, 3, 5], [2, 3, 8], [2, 4, 10], [3, 4, 2]],
        start: 0,
        target: 4
      },
      'Alternative Routes': {
        nodes: [0, 1, 2, 3],
        edges: [[0, 1, 10], [0, 2, 3], [2, 1, 4], [1, 3, 2], [2, 3, 8]],
        start: 0,
        target: 3
      }
    }
  },

  // Stretch Target: A* Pathfinding (Grid)
  astar: {
    name: 'A* Pathfinding (Grid)',
    category: 'graph',
    family: 'grid',
    time: 'O(b^d)',
    space: 'O(V)',
    description: 'Heuristic-driven search algorithm that finds the shortest path using f(n) = g(n) + h(n) on a 2D obstacle grid.',
    defaultInput: {
      rows: 6,
      cols: 8,
      start: [0, 0],
      target: [5, 7],
      walls: [[1, 2], [2, 2], [3, 2], [3, 4], [4, 4], [2, 5]]
    },
    presets: {
      'Simple Maze': {
        rows: 6,
        cols: 8,
        start: [0, 0],
        target: [5, 7],
        walls: [[1, 2], [2, 2], [3, 2], [3, 4], [4, 4], [2, 5]]
      },
      'Direct Path': {
        rows: 5,
        cols: 6,
        start: [0, 0],
        target: [4, 5],
        walls: [[1, 1], [2, 2], [3, 3]]
      }
    }
  },

  // Stretch Target: Binary Search Tree
  bst: {
    name: 'Binary Search Tree (BST)',
    category: 'tree',
    family: 'tree',
    time: 'O(log n) avg',
    space: 'O(n)',
    description: 'Demonstrates inserting values into a binary search tree and searching for a key by branching left or right.',
    defaultInput: {
      keys: [50, 30, 70, 20, 40, 60, 80],
      search_target: 40
    },
    presets: {
      'Balanced Tree': {
        keys: [50, 30, 70, 20, 40, 60, 80],
        search_target: 40
      },
      'Search Missing': {
        keys: [45, 25, 65, 15, 35, 75],
        search_target: 99
      }
    }
  }
};

// ============================================================================
// 2. STATE MANAGEMENT & PLAYBACK CONTROLLER
// ============================================================================

const state = {
  currentCategory: 'sorting',
  currentAlgorithmKey: 'bubble_sort',
  
  // Execution Data from C++
  rawResponse: null,
  initialDataSnapshot: null,
  steps: [],
  currentStepIndex: 0,
  
  // Playback Control
  isPlaying: false,
  timerId: null,
  playbackSpeed: 5, // 1 to 10
  
  // Engine health
  isEngineConnected: false
};

// ============================================================================
// 3. DOM ELEMENT REFERENCES
// ============================================================================

const DOM = {
  // Navigation & Category
  navTabs: document.querySelectorAll('.nav-tab'),
  engineStatusIndicator: document.getElementById('engineStatusIndicator'),
  engineStatusLabel: document.getElementById('engineStatusLabel'),
  themeToggleBtn: document.getElementById('themeToggleBtn'),
  themeSunIcon: document.getElementById('themeSunIcon'),
  themeMoonIcon: document.getElementById('themeMoonIcon'),
  
  // Algorithm Info & Select
  algorithmSelect: document.getElementById('algorithmSelect'),
  algoDescription: document.getElementById('algoDescription'),
  timeComplexity: document.getElementById('timeComplexity'),
  spaceComplexity: document.getElementById('spaceComplexity'),
  familyType: document.getElementById('familyType'),
  
  // Input Panel
  presetButtons: document.getElementById('presetButtons'),
  inputFormContainer: document.getElementById('inputFormContainer'),
  runBtn: document.getElementById('runBtn'),
  resetInputBtn: document.getElementById('resetInputBtn'),
  errorBanner: document.getElementById('errorBanner'),
  errorTitle: document.getElementById('errorTitle'),
  errorMessage: document.getElementById('errorMessage'),
  
  // Stage Metadata & Stats
  stageAlgoName: document.getElementById('stageAlgoName'),
  executionStateBadge: document.getElementById('executionStateBadge'),
  statTotalSteps: document.getElementById('statTotalSteps'),
  statComparisons: document.getElementById('statComparisons'),
  statSwaps: document.getElementById('statSwaps'),
  statComparisonsPill: document.getElementById('statComparisonsPill'),
  statSwapsPill: document.getElementById('statSwapsPill'),
  
  // Viewports
  arrayViewport: document.getElementById('arrayViewport'),
  graphViewport: document.getElementById('graphViewport'),
  gridViewport: document.getElementById('gridViewport'),
  treeViewport: document.getElementById('treeViewport'),
  
  // Containers
  arrayBarsContainer: document.getElementById('arrayBarsContainer'),
  graphSvg: document.getElementById('graphSvg'),
  graphEdgesGroup: document.getElementById('graphEdgesGroup'),
  graphNodesGroup: document.getElementById('graphNodesGroup'),
  auxQueueTitle: document.getElementById('auxQueueTitle'),
  auxQueueItems: document.getElementById('auxQueueItems'),
  auxDistanceBlock: document.getElementById('auxDistanceBlock'),
  auxDistanceItems: document.getElementById('auxDistanceItems'),
  gridContainer: document.getElementById('gridContainer'),
  treeSvg: document.getElementById('treeSvg'),
  treeLinksGroup: document.getElementById('treeLinksGroup'),
  treeNodesGroup: document.getElementById('treeNodesGroup'),
  treePathItems: document.getElementById('treePathItems'),
  
  // Explainer Banner & Legend
  eventTypeBadge: document.getElementById('eventTypeBadge'),
  eventMessageText: document.getElementById('eventMessageText'),
  stageLegend: document.getElementById('stageLegend'),
  
  // Playback Controls
  btnRestart: document.getElementById('btnRestart'),
  btnPrev: document.getElementById('btnPrev'),
  btnPlayPause: document.getElementById('btnPlayPause'),
  playIcon: document.getElementById('playIcon'),
  pauseIcon: document.getElementById('pauseIcon'),
  btnNext: document.getElementById('btnNext'),
  stepCounterText: document.getElementById('stepCounterText'),
  stepScrubber: document.getElementById('stepScrubber'),
  speedSlider: document.getElementById('speedSlider'),
  speedValueLabel: document.getElementById('speedValueLabel')
};

// ============================================================================
// 4. INITIALIZATION & LIFECYCLE
// ============================================================================

document.addEventListener('DOMContentLoaded', () => {
  initTheme();
  initEventListeners();
  checkEngineHealth();
  populateAlgorithmSelect();
  selectAlgorithm(state.currentAlgorithmKey);
});

function initEventListeners() {
  // Theme toggle button
  if (DOM.themeToggleBtn) {
    DOM.themeToggleBtn.addEventListener('click', toggleTheme);
  }

  // Category tabs
  DOM.navTabs.forEach(tab => {
    tab.addEventListener('click', () => {
      const category = tab.dataset.category;
      setCategory(category);
    });
  });

  // Algorithm select change
  DOM.algorithmSelect.addEventListener('change', (e) => {
    selectAlgorithm(e.target.value);
  });

  // Run button
  DOM.runBtn.addEventListener('click', handleRunAlgorithm);

  // Reset input button
  DOM.resetInputBtn.addEventListener('click', () => {
    const algo = ALGORITHMS[state.currentAlgorithmKey];
    populateInputForm(algo);
  });

  // Playback controls
  DOM.btnPlayPause.addEventListener('click', togglePlayPause);
  DOM.btnNext.addEventListener('click', stepForward);
  DOM.btnPrev.addEventListener('click', stepBackward);
  DOM.btnRestart.addEventListener('click', restartPlayback);
  
  // Scrubbing
  DOM.stepScrubber.addEventListener('input', (e) => {
    const stepIdx = parseInt(e.target.value, 10);
    jumpToStep(stepIdx);
  });

  // Speed slider
  DOM.speedSlider.addEventListener('input', (e) => {
    state.playbackSpeed = parseInt(e.target.value, 10);
    DOM.speedValueLabel.textContent = `${(state.playbackSpeed * 0.25 + 0.25).toFixed(1)}x`;
    if (state.isPlaying) {
      pausePlayback();
      startPlayback();
    }
  });

  // Keyboard Shortcuts
  window.addEventListener('keydown', (e) => {
    // Avoid triggering when focused in an input/textarea
    if (['INPUT', 'TEXTAREA', 'SELECT'].includes(document.activeElement.tagName)) return;
    
    if (e.code === 'Space') {
      e.preventDefault();
      togglePlayPause();
    } else if (e.code === 'ArrowRight') {
      e.preventDefault();
      stepForward();
    } else if (e.code === 'ArrowLeft') {
      e.preventDefault();
      stepBackward();
    } else if (e.code === 'Home') {
      e.preventDefault();
      restartPlayback();
    }
  });
}

// ============================================================================
// 5. ENGINE HEALTH CHECK & API COMMUNICATION
// ============================================================================

async function checkEngineHealth() {
  try {
    const response = await fetch('/api/health');
    if (response.ok) {
      const data = await response.json();
      state.isEngineConnected = true;
      DOM.engineStatusIndicator.className = 'status-indicator online';
      DOM.engineStatusLabel.textContent = `C++ Engine Connected (v${data.version || '1.0'})`;
    } else {
      throw new Error('Non-200 status');
    }
  } catch (err) {
    state.isEngineConnected = false;
    DOM.engineStatusIndicator.className = 'status-indicator offline';
    DOM.engineStatusLabel.textContent = 'C++ Engine Offline (Start Server)';
  }
}

// Periodically check engine connection every 8 seconds
setInterval(checkEngineHealth, 8000);

async function callVisualizeAPI(payload) {
  hideError();
  DOM.runBtn.disabled = true;
  DOM.runBtn.innerHTML = `
    <svg class="spin" viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2">
      <circle cx="12" cy="12" r="10" stroke-dasharray="32" stroke-linecap="round"></circle>
    </svg>
    <span>Executing C++...</span>
  `;

  try {
    const response = await fetch('/api/visualize', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });

    const data = await response.json();
    if (!response.ok) {
      const errorMsg = data.message || data.error || 'Server validation failed';
      showError('C++ Engine Error', errorMsg);
      return null;
    }

    return data;
  } catch (error) {
    showError('Connection Failed', `Could not reach C++ server at /api/visualize. Ensure ./algo_forge_server is running.`);
    return null;
  } finally {
    DOM.runBtn.disabled = false;
    DOM.runBtn.innerHTML = `
      <svg viewBox="0 0 24 24" width="18" height="18" fill="currentColor">
        <polygon points="5 3 19 12 5 21 5 3"></polygon>
      </svg>
      <span>Run C++ Engine</span>
    `;
  }
}

// ============================================================================
// 6. CATEGORY & ALGORITHM SELECTION
// ============================================================================

function setCategory(category) {
  state.currentCategory = category;
  
  DOM.navTabs.forEach(tab => {
    tab.classList.toggle('active', tab.dataset.category === category);
  });

  populateAlgorithmSelect();

  // Pick first algorithm in category
  const firstAlgoKey = Object.keys(ALGORITHMS).find(k => ALGORITHMS[k].category === category);
  if (firstAlgoKey) {
    selectAlgorithm(firstAlgoKey);
  }
}

function populateAlgorithmSelect() {
  DOM.algorithmSelect.innerHTML = '';
  
  Object.entries(ALGORITHMS).forEach(([key, algo]) => {
    if (algo.category === state.currentCategory) {
      const opt = document.createElement('option');
      opt.value = key;
      opt.textContent = algo.name;
      DOM.algorithmSelect.appendChild(opt);
    }
  });
}

function selectAlgorithm(algoKey) {
  const algo = ALGORITHMS[algoKey];
  if (!algo) return;

  pausePlayback();
  state.currentAlgorithmKey = algoKey;
  DOM.algorithmSelect.value = algoKey;

  // Update Metadata
  DOM.stageAlgoName.textContent = algo.name;
  DOM.algoDescription.textContent = algo.description;
  DOM.timeComplexity.textContent = algo.time;
  DOM.spaceComplexity.textContent = algo.space;
  DOM.familyType.textContent = capitalize(algo.family.replace('_', ' '));

  // Reset Stats
  DOM.executionStateBadge.textContent = 'Ready';
  DOM.executionStateBadge.className = 'stage-status-badge';
  DOM.statTotalSteps.textContent = '0';
  DOM.statComparisons.textContent = '0';
  DOM.statSwaps.textContent = '0';

  // Toggle Visibility of Stats
  const isSort = algo.category === 'sorting';
  DOM.statComparisonsPill.style.display = isSort ? 'flex' : 'none';
  DOM.statSwapsPill.style.display = isSort ? 'flex' : 'none';

  // Update Legend
  renderLegend(algo);

  // Setup Dynamic Inputs & Presets
  setupPresets(algo);
  populateInputForm(algo);

  // Activate appropriate Viewport
  activateViewport(algo.family);

  // Render initial static preview
  renderInitialPreview(algo);

  // Event banner prompt
  DOM.eventTypeBadge.textContent = 'READY';
  DOM.eventMessageText.textContent = `Configured for ${algo.name}. Click "Run C++ Engine" to record real execution steps.`;
}

function activateViewport(family) {
  DOM.arrayViewport.classList.remove('active');
  DOM.graphViewport.classList.remove('active');
  DOM.gridViewport.classList.remove('active');
  DOM.treeViewport.classList.remove('active');

  if (family === 'array' || family === 'search_array') {
    DOM.arrayViewport.classList.add('active');
  } else if (family === 'graph' || family === 'weighted_graph') {
    DOM.graphViewport.classList.add('active');
  } else if (family === 'grid') {
    DOM.gridViewport.classList.add('active');
  } else if (family === 'tree') {
    DOM.treeViewport.classList.add('active');
  }
}

// ============================================================================
// 7. INPUT FORMS & PRESETS
// ============================================================================

function setupPresets(algo) {
  DOM.presetButtons.innerHTML = '';
  if (!algo.presets) return;

  Object.entries(algo.presets).forEach(([label, presetVal]) => {
    const btn = document.createElement('button');
    btn.className = 'preset-chip';
    btn.textContent = label;
    btn.type = 'button';
    btn.addEventListener('click', () => {
      const resolved = typeof presetVal === 'function' ? presetVal() : presetVal;
      applyPreset(algo, resolved);
    });
    DOM.presetButtons.appendChild(btn);
  });
}

function applyPreset(algo, presetVal) {
  if (algo.family === 'array') {
    const input = document.getElementById('arrayInput');
    if (input) input.value = presetVal.join(', ');
  } else if (algo.family === 'search_array') {
    const arrayInput = document.getElementById('searchArrayInput');
    const targetInput = document.getElementById('searchTargetInput');
    if (arrayInput) arrayInput.value = presetVal.values.join(', ');
    if (targetInput) targetInput.value = presetVal.target;
  } else if (algo.family === 'graph' || algo.family === 'weighted_graph') {
    const edgesInput = document.getElementById('graphEdgesInput');
    const startInput = document.getElementById('graphStartInput');
    const targetInput = document.getElementById('graphTargetInput');
    if (edgesInput) edgesInput.value = JSON.stringify(presetVal.edges);
    if (startInput) startInput.value = presetVal.start;
    if (targetInput && presetVal.target !== undefined) targetInput.value = presetVal.target;
  } else if (algo.family === 'grid') {
    const wallsInput = document.getElementById('gridWallsInput');
    if (wallsInput) wallsInput.value = JSON.stringify(presetVal.walls);
  } else if (algo.family === 'tree') {
    const keysInput = document.getElementById('treeKeysInput');
    const targetInput = document.getElementById('treeTargetInput');
    if (keysInput) keysInput.value = presetVal.keys.join(', ');
    if (targetInput) targetInput.value = presetVal.search_target;
  }
}

function populateInputForm(algo) {
  const container = DOM.inputFormContainer;
  container.innerHTML = '';

  if (algo.family === 'array') {
    container.innerHTML = `
      <div class="form-group">
        <label class="form-label" for="arrayInput">
          <span>Array Elements</span>
          <span class="form-hint">Comma-separated (3–16 items)</span>
        </label>
        <input type="text" id="arrayInput" class="styled-input" value="${algo.defaultInput.values.join(', ')}">
      </div>
    `;
  } else if (algo.family === 'search_array') {
    container.innerHTML = `
      <div class="form-group">
        <label class="form-label" for="searchArrayInput">
          <span>Array Elements</span>
          <span class="form-hint">${algoKeyIsBinarySearch() ? 'Must be sorted' : 'Comma-separated'}</span>
        </label>
        <input type="text" id="searchArrayInput" class="styled-input" value="${algo.defaultInput.values.join(', ')}">
      </div>
      <div class="form-group">
        <label class="form-label" for="searchTargetInput">Search Target Value</label>
        <input type="number" id="searchTargetInput" class="styled-input" value="${algo.defaultInput.target}">
      </div>
    `;
  } else if (algo.family === 'graph' || algo.family === 'weighted_graph') {
    const isWeighted = algo.family === 'weighted_graph';
    container.innerHTML = `
      <div class="form-group">
        <label class="form-label" for="graphNodesInput">
          <span>Nodes</span>
          <span class="form-hint">0 to N-1 IDs</span>
        </label>
        <input type="text" id="graphNodesInput" class="styled-input" value="${algo.defaultInput.nodes.join(', ')}">
      </div>
      <div class="form-group">
        <label class="form-label" for="graphEdgesInput">
          <span>Edges JSON ${isWeighted ? '[u, v, weight]' : '[u, v]'}</span>
        </label>
        <textarea id="graphEdgesInput" class="styled-textarea">${JSON.stringify(algo.defaultInput.edges)}</textarea>
      </div>
      <div class="form-row">
        <div class="form-group">
          <label class="form-label" for="graphStartInput">Start Node</label>
          <input type="number" id="graphStartInput" class="styled-input" value="${algo.defaultInput.start}">
        </div>
        ${isWeighted ? `
        <div class="form-group">
          <label class="form-label" for="graphTargetInput">Target Node</label>
          <input type="number" id="graphTargetInput" class="styled-input" value="${algo.defaultInput.target}">
        </div>` : ''}
      </div>
    `;
  } else if (algo.family === 'grid') {
    container.innerHTML = `
      <div class="form-row">
        <div class="form-group">
          <label class="form-label">Dimensions</label>
          <input type="text" id="gridDimInput" class="styled-input" value="6 rows x 8 cols" readonly>
        </div>
        <div class="form-group">
          <label class="form-label">Start / Target</label>
          <input type="text" id="gridEndpointsInput" class="styled-input" value="[0,0] -> [5,7]" readonly>
        </div>
      </div>
      <div class="form-group">
        <label class="form-label" for="gridWallsInput">
          <span>Obstacles / Walls [[r, c], ...]</span>
        </label>
        <textarea id="gridWallsInput" class="styled-textarea">${JSON.stringify(algo.defaultInput.walls)}</textarea>
      </div>
    `;
  } else if (algo.family === 'tree') {
    container.innerHTML = `
      <div class="form-group">
        <label class="form-label" for="treeKeysInput">
          <span>Keys to Insert (Comma-separated)</span>
        </label>
        <input type="text" id="treeKeysInput" class="styled-input" value="${algo.defaultInput.keys.join(', ')}">
      </div>
      <div class="form-group">
        <label class="form-label" for="treeTargetInput">Target Key to Search</label>
        <input type="number" id="treeTargetInput" class="styled-input" value="${algo.defaultInput.search_target}">
      </div>
    `;
  }
}

function parseCurrentInput() {
  const algo = ALGORITHMS[state.currentAlgorithmKey];
  
  if (algo.family === 'array') {
    const raw = document.getElementById('arrayInput').value;
    const values = raw.split(',').map(s => parseInt(s.trim(), 10)).filter(n => !isNaN(n));
    if (values.length < 2) throw new Error('Please enter at least 2 numbers.');
    if (values.length > 24) throw new Error('Array maximum size is 24 elements for clear visualization.');
    return { values };
  }
  
  if (algo.family === 'search_array') {
    const raw = document.getElementById('searchArrayInput').value;
    const values = raw.split(',').map(s => parseInt(s.trim(), 10)).filter(n => !isNaN(n));
    const target = parseInt(document.getElementById('searchTargetInput').value, 10);
    if (values.length < 2) throw new Error('Please enter at least 2 numbers.');
    if (isNaN(target)) throw new Error('Please enter a valid target integer.');
    return { values, target };
  }
  
  if (algo.family === 'graph' || algo.family === 'weighted_graph') {
    const nodesRaw = document.getElementById('graphNodesInput').value;
    const nodes = nodesRaw.split(',').map(s => parseInt(s.trim(), 10)).filter(n => !isNaN(n));
    const edges = JSON.parse(document.getElementById('graphEdgesInput').value);
    const start = parseInt(document.getElementById('graphStartInput').value, 10);
    const targetElem = document.getElementById('graphTargetInput');
    const target = targetElem ? parseInt(targetElem.value, 10) : undefined;
    
    if (nodes.length < 2) throw new Error('Graph must have at least 2 nodes.');
    if (!Array.isArray(edges)) throw new Error('Edges must be a valid JSON array.');
    if (isNaN(start) || !nodes.includes(start)) throw new Error(`Start node (${start}) does not exist in nodes list.`);
    if (target !== undefined && (isNaN(target) || !nodes.includes(target))) {
      throw new Error(`Target node (${target}) does not exist in nodes list.`);
    }
    
    return { nodes, edges, start, ...(target !== undefined ? { target } : {}) };
  }
  
  if (algo.family === 'grid') {
    const walls = JSON.parse(document.getElementById('gridWallsInput').value);
    return {
      rows: 6,
      cols: 8,
      start: [0, 0],
      target: [5, 7],
      walls: Array.isArray(walls) ? walls : []
    };
  }
  
  if (algo.family === 'tree') {
    const raw = document.getElementById('treeKeysInput').value;
    const keys = raw.split(',').map(s => parseInt(s.trim(), 10)).filter(n => !isNaN(n));
    const target = parseInt(document.getElementById('treeTargetInput').value, 10);
    if (keys.length < 1) throw new Error('Enter at least 1 key to insert.');
    if (isNaN(target)) throw new Error('Enter a valid target key to search.');
    return { keys, search_target: target };
  }

  return {};
}

// ============================================================================
// 8. RUN ENGINE EXECUTION
// ============================================================================

async function handleRunAlgorithm() {
  pausePlayback();
  hideError();

  let inputData;
  try {
    inputData = parseCurrentInput();
  } catch (err) {
    showError('Input Validation Error', err.message);
    return;
  }

  const payload = {
    algorithm: state.currentAlgorithmKey,
    input: inputData
  };

  const response = await callVisualizeAPI(payload);
  if (!response) return;

  // Store response and initialize playback state
  state.rawResponse = response;
  state.initialDataSnapshot = JSON.parse(JSON.stringify(inputData));
  state.steps = response.steps || [];
  state.currentStepIndex = 0;

  // Update Stats
  DOM.statTotalSteps.textContent = state.steps.length;
  if (response.metadata) {
    if (response.metadata.comparisons !== undefined) {
      DOM.statComparisons.textContent = response.metadata.comparisons;
    }
    if (response.metadata.swaps !== undefined) {
      DOM.statSwaps.textContent = response.metadata.swaps;
    }
  }

  DOM.executionStateBadge.textContent = 'Recorded';
  DOM.executionStateBadge.className = 'stage-status-badge';

  // Configure scrubber
  DOM.stepScrubber.max = state.steps.length;
  DOM.stepScrubber.value = 0;
  updateStepIndicator();

  // Apply step 0 (initial state)
  applyPlaybackStep(0);

  // Auto-play automatically after run
  startPlayback();
}

// ============================================================================
// 9. PLAYBACK ENGINE & STEP DISPATCHER
// ============================================================================

function togglePlayPause() {
  if (state.steps.length === 0) return;
  if (state.isPlaying) {
    pausePlayback();
  } else {
    // If reached end, restart
    if (state.currentStepIndex >= state.steps.length) {
      restartPlayback();
    }
    startPlayback();
  }
}

function startPlayback() {
  if (state.steps.length === 0) return;
  state.isPlaying = true;
  DOM.playIcon.classList.add('hidden');
  DOM.pauseIcon.classList.remove('hidden');
  DOM.executionStateBadge.textContent = 'Playing';
  DOM.executionStateBadge.className = 'stage-status-badge';

  const delayMs = getDelayForSpeed(state.playbackSpeed);
  
  const stepLoop = () => {
    if (!state.isPlaying) return;
    
    if (state.currentStepIndex < state.steps.length) {
      stepForward();
      state.timerId = setTimeout(stepLoop, getDelayForSpeed(state.playbackSpeed));
    } else {
      pausePlayback();
      DOM.executionStateBadge.textContent = 'Done';
    }
  };

  state.timerId = setTimeout(stepLoop, delayMs);
}

function pausePlayback() {
  state.isPlaying = false;
  if (state.timerId) {
    clearTimeout(state.timerId);
    state.timerId = null;
  }
  DOM.playIcon.classList.remove('hidden');
  DOM.pauseIcon.classList.add('hidden');
  if (state.currentStepIndex > 0 && state.currentStepIndex < state.steps.length) {
    DOM.executionStateBadge.textContent = 'Paused';
  }
}

function stepForward() {
  if (state.currentStepIndex < state.steps.length) {
    state.currentStepIndex++;
    applyPlaybackStep(state.currentStepIndex);
    updateStepIndicator();
  } else {
    pausePlayback();
  }
}

function stepBackward() {
  pausePlayback();
  if (state.currentStepIndex > 0) {
    state.currentStepIndex--;
    applyPlaybackStep(state.currentStepIndex);
    updateStepIndicator();
  }
}

function restartPlayback() {
  pausePlayback();
  state.currentStepIndex = 0;
  applyPlaybackStep(0);
  updateStepIndicator();
  DOM.executionStateBadge.textContent = 'Restarted';
}

function jumpToStep(targetStep) {
  pausePlayback();
  state.currentStepIndex = Math.max(0, Math.min(targetStep, state.steps.length));
  applyPlaybackStep(state.currentStepIndex);
  updateStepIndicator();
}

function updateStepIndicator() {
  DOM.stepCounterText.textContent = `Step ${state.currentStepIndex} / ${state.steps.length}`;
  DOM.stepScrubber.value = state.currentStepIndex;
}

function getDelayForSpeed(speed) {
  // speed 1 (slowest, 1200ms) to 10 (fastest, 60ms)
  const delays = [1200, 950, 750, 550, 400, 280, 180, 120, 80, 50];
  const idx = Math.max(0, Math.min(speed - 1, delays.length - 1));
  return delays[idx];
}

// ============================================================================
// 10. STEP REPLAY & VIEWPORT RENDERERS
// ============================================================================

/**
 * Replays state up to stepIndex deterministically.
 */
function applyPlaybackStep(stepIndex) {
  const algo = ALGORITHMS[state.currentAlgorithmKey];
  const isInitial = (stepIndex === 0);
  const currentStep = isInitial ? null : state.steps[stepIndex - 1];

  // Update Event Explainer Banner
  if (isInitial) {
    DOM.eventTypeBadge.textContent = 'START';
    DOM.eventMessageText.textContent = 'Initial state before execution.';
  } else if (currentStep) {
    DOM.eventTypeBadge.textContent = (currentStep.type || 'INFO').toUpperCase();
    DOM.eventMessageText.textContent = currentStep.message || `Executing step ${stepIndex}`;
  }

  // Dispatch renderer based on family
  if (algo.family === 'array' || algo.family === 'search_array') {
    replayArrayState(stepIndex, currentStep);
  } else if (algo.family === 'graph' || algo.family === 'weighted_graph') {
    replayGraphState(stepIndex, currentStep);
  } else if (algo.family === 'grid') {
    replayGridState(stepIndex, currentStep);
  } else if (algo.family === 'tree') {
    replayTreeState(stepIndex, currentStep);
  }
}

// ----------------------------------------------------------------------------
// A. Array Bar Renderer
// ----------------------------------------------------------------------------

function renderInitialPreview(algo) {
  if (algo.family === 'array' || algo.family === 'search_array') {
    const values = algo.defaultInput.values;
    renderArrayBars(values, {});
  } else if (algo.family === 'graph' || algo.family === 'weighted_graph') {
    renderGraph(algo.defaultInput.nodes, algo.defaultInput.edges, {}, {});
  } else if (algo.family === 'grid') {
    renderGrid(algo.defaultInput.rows, algo.defaultInput.cols, algo.defaultInput.walls, {}, []);
  } else if (algo.family === 'tree') {
    renderTree(algo.defaultInput.keys, {}, []);
  }
}

function replayArrayState(stepIndex, currentStep) {
  if (!state.initialDataSnapshot) return;
  
  // Reconstruct array state up to stepIndex
  const arr = [...state.initialDataSnapshot.values];
  const states = {}; // index -> 'compare' | 'swap' | 'pivot' | 'sorted' | 'active'

  for (let i = 0; i < stepIndex; i++) {
    const step = state.steps[i];
    if (!step) continue;
    
    // Apply state modifications from events
    if (step.type === 'swap' && step.indices && step.indices.length === 2) {
      const [idx1, idx2] = step.indices;
      const tmp = arr[idx1];
      arr[idx1] = arr[idx2];
      arr[idx2] = tmp;
    } else if (step.type === 'write' && step.indices && step.indices.length >= 1) {
      const idx = step.indices[0];
      if (step.values && step.values.length > 0) {
        arr[idx] = step.values[0];
      }
    }
  }

  // Apply active highlights for current step
  if (currentStep) {
    const type = currentStep.type;
    const indices = currentStep.indices || [];
    
    if (type === 'compare') {
      indices.forEach(idx => states[idx] = 'compare');
    } else if (type === 'swap') {
      indices.forEach(idx => states[idx] = 'swap');
    } else if (type === 'pivot') {
      indices.forEach(idx => states[idx] = 'pivot');
    } else if (type === 'check') {
      indices.forEach(idx => states[idx] = 'active');
    } else if (type === 'found') {
      indices.forEach(idx => states[idx] = 'sorted');
    } else if (type === 'done') {
      arr.forEach((_, idx) => states[idx] = 'sorted');
    } else if (type === 'window' && indices.length >= 2) {
      const [low, high] = indices;
      for (let k = low; k <= high; k++) {
        states[k] = 'low-high';
      }
      if (indices.length >= 3) {
        states[indices[2]] = 'active'; // mid
      }
    }
  }

  renderArrayBars(arr, states);
}

function renderArrayBars(arr, states) {
  const container = DOM.arrayBarsContainer;
  container.innerHTML = '';

  const maxVal = Math.max(...arr, 1);
  const minVal = Math.min(...arr, 0);
  const range = Math.max(maxVal - minVal, 1);

  arr.forEach((val, idx) => {
    const wrapper = document.createElement('div');
    wrapper.className = 'bar-wrapper';
    if (states[idx]) {
      wrapper.classList.add(`state-${states[idx]}`);
    }

    const heightPct = Math.max(((val - minVal) / range) * 88 + 12, 10);

    wrapper.innerHTML = `
      <span class="bar-value">${val}</span>
      <div class="bar-element" style="height: ${heightPct}%"></div>
      <span class="bar-index">[${idx}]</span>
    `;

    container.appendChild(wrapper);
  });
}

// ----------------------------------------------------------------------------
// B. Graph Renderer (Canvas / SVG)
// ----------------------------------------------------------------------------

function replayGraphState(stepIndex, currentStep) {
  if (!state.initialDataSnapshot) return;
  
  const nodes = state.initialDataSnapshot.nodes;
  const edges = state.initialDataSnapshot.edges;
  
  const nodeStates = {}; // id -> 'visited' | 'active' | 'frontier' | 'path'
  const edgeStates = {}; // 'u-v' -> 'active' | 'path'
  let frontierSnapshot = [];
  let distancesSnapshot = {};

  for (let i = 0; i < stepIndex; i++) {
    const step = state.steps[i];
    if (!step) continue;
    
    if (step.type === 'visit' && step.node !== undefined) {
      nodeStates[step.node] = 'visited';
    } else if (step.type === 'enqueue' && step.node !== undefined) {
      nodeStates[step.node] = 'frontier';
    } else if (step.type === 'relax' && step.indices && step.indices.length >= 2) {
      const [u, v] = step.indices;
      edgeStates[`${u}-${v}`] = 'active';
      edgeStates[`${v}-${u}`] = 'active';
    } else if (step.type === 'path' && step.path) {
      step.path.forEach(nid => nodeStates[nid] = 'path');
      for (let k = 0; k < step.path.length - 1; k++) {
        const u = step.path[k], v = step.path[k + 1];
        edgeStates[`${u}-${v}`] = 'path';
        edgeStates[`${v}-${u}`] = 'path';
      }
    }
  }

  // Active highlights from current step
  if (currentStep) {
    if (currentStep.node !== undefined) {
      nodeStates[currentStep.node] = 'active';
    }
    if (currentStep.queue) frontierSnapshot = currentStep.queue;
    if (currentStep.distances) distancesSnapshot = currentStep.distances;
    if (currentStep.path) {
      currentStep.path.forEach(nid => nodeStates[nid] = 'path');
    }
  }

  // Update Aux Panels
  DOM.auxQueueTitle.textContent = state.currentAlgorithmKey === 'dfs' ? 'Call Stack:' : 'Queue / Frontier:';
  DOM.auxQueueItems.textContent = frontierSnapshot.length > 0 ? `[ ${frontierSnapshot.join(' → ')} ]` : 'Empty';

  if (state.currentAlgorithmKey === 'dijkstra') {
    DOM.auxDistanceBlock.style.display = 'flex';
    const distText = Object.entries(distancesSnapshot)
      .map(([k, d]) => `${k}:${d === 999999 ? '∞' : d}`)
      .join(', ');
    DOM.auxDistanceItems.textContent = distText || 'Initializing...';
  } else {
    DOM.auxDistanceBlock.style.display = 'none';
  }

  renderGraph(nodes, edges, nodeStates, edgeStates);
}

function renderGraph(nodes, edges, nodeStates, edgeStates) {
  const nodesGroup = DOM.graphNodesGroup;
  const edgesGroup = DOM.graphEdgesGroup;
  nodesGroup.innerHTML = '';
  edgesGroup.innerHTML = '';

  const width = 800;
  const height = 440;
  const cx = width / 2;
  const cy = height / 2;
  const radius = Math.min(cx, cy) - 60;

  // Compute circular layout for nodes
  const coords = {};
  const N = nodes.length;
  nodes.forEach((nodeId, idx) => {
    const angle = (idx / N) * 2 * Math.PI - Math.PI / 2;
    coords[nodeId] = {
      x: cx + radius * Math.cos(angle),
      y: cy + radius * Math.sin(angle)
    };
  });

  // Render Edges
  edges.forEach(edge => {
    const u = edge[0];
    const v = edge[1];
    const w = edge.length > 2 ? edge[2] : null;

    if (!coords[u] || !coords[v]) return;

    const line = document.createElementNS('http://www.w3.org/2000/svg', 'line');
    line.setAttribute('x1', coords[u].x);
    line.setAttribute('y1', coords[u].y);
    line.setAttribute('x2', coords[v].x);
    line.setAttribute('y2', coords[v].y);
    line.setAttribute('class', 'graph-edge');

    const key = `${u}-${v}`;
    if (edgeStates[key]) {
      line.classList.add(`edge-${edgeStates[key]}`);
    }

    edgesGroup.appendChild(line);

    // If weighted, render weight badge
    if (w !== null) {
      const midX = (coords[u].x + coords[v].x) / 2;
      const midY = (coords[u].y + coords[v].y) / 2;
      const text = document.createElementNS('http://www.w3.org/2000/svg', 'text');
      text.setAttribute('x', midX);
      text.setAttribute('y', midY - 6);
      text.setAttribute('class', 'graph-edge-weight');
      text.textContent = w;
      edgesGroup.appendChild(text);
    }
  });

  // Render Nodes
  nodes.forEach(nodeId => {
    const { x, y } = coords[nodeId];
    const g = document.createElementNS('http://www.w3.org/2000/svg', 'g');
    g.setAttribute('class', 'graph-node');
    g.setAttribute('transform', `translate(${x}, ${y})`);

    if (nodeStates[nodeId]) {
      g.classList.add(`node-${nodeStates[nodeId]}`);
    }

    const circle = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
    circle.setAttribute('r', '22');

    const text = document.createElementNS('http://www.w3.org/2000/svg', 'text');
    text.textContent = nodeId;

    g.appendChild(circle);
    g.appendChild(text);
    nodesGroup.appendChild(g);
  });
}

// ----------------------------------------------------------------------------
// C. Grid / A* Pathfinding Renderer
// ----------------------------------------------------------------------------

function replayGridState(stepIndex, currentStep) {
  if (!state.initialDataSnapshot) return;
  
  const { rows, cols, start, target, walls } = state.initialDataSnapshot;
  const cellStates = {}; // 'r-c' -> 'open' | 'closed' | 'path'
  let finalPath = [];

  for (let i = 0; i < stepIndex; i++) {
    const step = state.steps[i];
    if (!step) continue;
    
    if (step.type === 'open' && step.indices) {
      cellStates[`${step.indices[0]}-${step.indices[1]}`] = 'open';
    } else if (step.type === 'close' && step.indices) {
      cellStates[`${step.indices[0]}-${step.indices[1]}`] = 'closed';
    } else if (step.type === 'path' && step.path) {
      finalPath = step.path;
    }
  }

  if (currentStep && currentStep.path) {
    finalPath = currentStep.path;
  }

  renderGrid(rows, cols, walls, cellStates, finalPath, start, target);
}

function renderGrid(rows, cols, walls, cellStates, finalPath = [], start = [0, 0], target = [rows - 1, cols - 1]) {
  const container = DOM.gridContainer;
  container.innerHTML = '';
  container.style.gridTemplateColumns = `repeat(${cols}, 28px)`;

  const wallSet = new Set(walls.map(w => `${w[0]}-${w[1]}`));
  const pathSet = new Set(finalPath.map(p => `${p[0]}-${p[1]}`));

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      const key = `${r}-${c}`;
      const cell = document.createElement('div');
      cell.className = 'grid-cell';

      if (r === start[0] && c === start[1]) {
        cell.classList.add('cell-start');
        cell.textContent = 'S';
      } else if (r === target[0] && c === target[1]) {
        cell.classList.add('cell-target');
        cell.textContent = 'T';
      } else if (wallSet.has(key)) {
        cell.classList.add('cell-wall');
      } else if (pathSet.has(key)) {
        cell.classList.add('cell-path');
      } else if (cellStates[key]) {
        cell.classList.add(`cell-${cellStates[key]}`);
      }

      container.appendChild(cell);
    }
  }
}

// ----------------------------------------------------------------------------
// D. Tree (BST) Renderer
// ----------------------------------------------------------------------------

function replayTreeState(stepIndex, currentStep) {
  if (!state.initialDataSnapshot) return;
  
  const insertedKeys = [];
  const highlightedNodes = {};
  let pathText = [];

  for (let i = 0; i < stepIndex; i++) {
    const step = state.steps[i];
    if (!step) continue;
    
    if (step.type === 'insert' && step.values && step.values.length > 0) {
      insertedKeys.push(step.values[0]);
    } else if (step.type === 'search_step' && step.values && step.values.length > 0) {
      highlightedNodes[step.values[0]] = 'active';
      pathText.push(step.values[0]);
    } else if (step.type === 'found' && step.values && step.values.length > 0) {
      highlightedNodes[step.values[0]] = 'sorted';
    }
  }

  if (currentStep) {
    if (currentStep.values && currentStep.values.length > 0) {
      highlightedNodes[currentStep.values[0]] = currentStep.type === 'found' ? 'sorted' : 'active';
    }
  }

  DOM.treePathItems.textContent = pathText.length > 0 ? pathText.join(' → ') : 'None';
  renderTree(insertedKeys.length > 0 ? insertedKeys : state.initialDataSnapshot.keys, highlightedNodes);
}

function renderTree(keys, nodeHighlights = {}) {
  const nodesGroup = DOM.treeNodesGroup;
  const linksGroup = DOM.treeLinksGroup;
  nodesGroup.innerHTML = '';
  linksGroup.innerHTML = '';

  if (!keys || keys.length === 0) return;

  // Build binary search tree structure
  class TreeNode {
    constructor(val) {
      this.val = val;
      this.left = null;
      this.right = null;
      this.x = 0;
      this.y = 0;
    }
  }

  let root = new TreeNode(keys[0]);
  for (let i = 1; i < keys.length; i++) {
    insertNode(root, keys[i]);
  }

  function insertNode(curr, val) {
    if (val < curr.val) {
      if (!curr.left) curr.left = new TreeNode(val);
      else insertNode(curr.left, val);
    } else if (val > curr.val) {
      if (!curr.right) curr.right = new TreeNode(val);
      else insertNode(curr.right, val);
    }
  }

  // Position nodes hierarchically
  const width = 800;
  function layout(node, depth, minX, maxX) {
    if (!node) return;
    node.x = (minX + maxX) / 2;
    node.y = 50 + depth * 70;
    layout(node.left, depth + 1, minX, node.x);
    layout(node.right, depth + 1, node.x, maxX);
  }

  layout(root, 0, 40, width - 40);

  // Render tree links
  function drawLinks(node) {
    if (!node) return;
    if (node.left) {
      drawLine(node.x, node.y, node.left.x, node.left.y);
      drawLinks(node.left);
    }
    if (node.right) {
      drawLine(node.x, node.y, node.right.x, node.right.y);
      drawLinks(node.right);
    }
  }

  function drawLine(x1, y1, x2, y2) {
    const line = document.createElementNS('http://www.w3.org/2000/svg', 'line');
    line.setAttribute('x1', x1);
    line.setAttribute('y1', y1);
    line.setAttribute('x2', x2);
    line.setAttribute('y2', y2);
    line.setAttribute('class', 'graph-edge');
    linksGroup.appendChild(line);
  }

  drawLinks(root);

  // Render tree nodes
  function drawNodes(node) {
    if (!node) return;
    const g = document.createElementNS('http://www.w3.org/2000/svg', 'g');
    g.setAttribute('class', 'graph-node');
    g.setAttribute('transform', `translate(${node.x}, ${node.y})`);

    if (nodeHighlights[node.val]) {
      g.classList.add(`node-${nodeHighlights[node.val]}`);
    }

    const circle = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
    circle.setAttribute('r', '20');

    const text = document.createElementNS('http://www.w3.org/2000/svg', 'text');
    text.textContent = node.val;

    g.appendChild(circle);
    g.appendChild(text);
    nodesGroup.appendChild(g);

    drawNodes(node.left);
    drawNodes(node.right);
  }

  drawNodes(root);
}

// ============================================================================
// 11. DYNAMIC LEGEND BUILDER
// ============================================================================

function renderLegend(algo) {
  const container = DOM.stageLegend;
  container.innerHTML = '';

  let swatches = [];
  if (algo.family === 'array') {
    swatches = [
      { label: 'Default Element', color: '#3b82f6' },
      { label: 'Comparing', color: '#f59e0b' },
      { label: 'Swapping / Shifting', color: '#ef4444' },
      ...(algo.name.includes('Quick') ? [{ label: 'Pivot', color: '#a855f7' }] : []),
      { label: 'Sorted / Final', color: '#10b981' }
    ];
  } else if (algo.family === 'search_array') {
    swatches = [
      { label: 'Array Element', color: '#3b82f6' },
      { label: 'Active Check / Mid', color: '#38bdf8' },
      { label: 'Search Window', color: '#06b6d4' },
      { label: 'Target Found', color: '#10b981' }
    ];
  } else if (algo.family === 'graph' || algo.family === 'weighted_graph') {
    swatches = [
      { label: 'Unvisited Node', color: '#475569' },
      { label: 'Frontier / Enqueued', color: '#06b6d4' },
      { label: 'Visited Node', color: '#6366f1' },
      { label: 'Active Node', color: '#38bdf8' },
      { label: 'Shortest Path', color: '#10b981' }
    ];
  } else if (algo.family === 'grid') {
    swatches = [
      { label: 'Start / Target', color: '#38bdf8' },
      { label: 'Wall Obstacle', color: '#334155' },
      { label: 'Open Set', color: '#06b6d4' },
      { label: 'Closed Set', color: '#6366f1' },
      { label: 'Final Path', color: '#10b981' }
    ];
  } else if (algo.family === 'tree') {
    swatches = [
      { label: 'Tree Node', color: '#475569' },
      { label: 'Active Node / Probe', color: '#38bdf8' },
      { label: 'Key Found', color: '#10b981' }
    ];
  }

  swatches.forEach(s => {
    const item = document.createElement('div');
    item.className = 'legend-item';
    item.innerHTML = `
      <span class="legend-color-box" style="background: ${s.color}"></span>
      <span>${s.label}</span>
    `;
    container.appendChild(item);
  });
}

// ============================================================================
// 12. UTILITY & HELPER FUNCTIONS
// ============================================================================

function showError(title, msg) {
  DOM.errorTitle.textContent = title;
  DOM.errorMessage.textContent = msg;
  DOM.errorBanner.classList.remove('hidden');
}

function hideError() {
  DOM.errorBanner.classList.add('hidden');
}

function algoKeyIsBinarySearch() {
  return state.currentAlgorithmKey === 'binary_search';
}

function generateRandomArray(size, min, max) {
  const arr = [];
  for (let i = 0; i < size; i++) {
    arr.push(Math.floor(Math.random() * (max - min + 1)) + min);
  }
  return arr;
}

function capitalize(str) {
  return str.charAt(0).toUpperCase() + str.slice(1);
}

// ============================================================================
// 13. THEME MANAGEMENT (LIGHT & DARK MODE)
// ============================================================================

function initTheme() {
  const savedTheme = localStorage.getItem('algo_forge_theme');
  const prefersLight = window.matchMedia && window.matchMedia('(prefers-color-scheme: light)').matches;
  const initialTheme = savedTheme || (prefersLight ? 'light' : 'dark');
  applyTheme(initialTheme);
}

function applyTheme(theme) {
  document.documentElement.setAttribute('data-theme', theme);
  localStorage.setItem('algo_forge_theme', theme);

  if (DOM.themeSunIcon && DOM.themeMoonIcon && DOM.themeToggleBtn) {
    if (theme === 'light') {
      DOM.themeSunIcon.classList.add('hidden');
      DOM.themeMoonIcon.classList.remove('hidden');
      DOM.themeToggleBtn.setAttribute('title', 'Switch to dark mode');
      DOM.themeToggleBtn.setAttribute('aria-label', 'Switch to dark mode');
    } else {
      DOM.themeSunIcon.classList.remove('hidden');
      DOM.themeMoonIcon.classList.add('hidden');
      DOM.themeToggleBtn.setAttribute('title', 'Switch to light mode');
      DOM.themeToggleBtn.setAttribute('aria-label', 'Switch to light mode');
    }
  }
}

function toggleTheme() {
  const current = document.documentElement.getAttribute('data-theme') || 'dark';
  const nextTheme = current === 'dark' ? 'light' : 'dark';
  applyTheme(nextTheme);
}
