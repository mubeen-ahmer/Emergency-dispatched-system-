# ERDS Lite — Emergency Response Dispatch System

A console-based C++ application simulating a centralized emergency dispatch system. All data structures (heap, hash table, graph, AVL tree, queue, stack) are implemented from scratch — no STL containers used.

## Overview

The system manages incoming emergency incidents, dispatches the nearest available vehicle using BFS on a city map graph, and maintains a full historical log of resolved incidents with file persistence across sessions.

```bash
g++ main.cpp -o ERDS
./ERDS
```

## Data Structures Used

| Module | Structure | Purpose |
|---|---|---|
| `IncidentMinHeap` | Min-Heap (array-based) | Prioritize incidents by urgency level |
| `VehicleHashTable` | Hash Table + Chaining | O(1) vehicle lookup and status update |
| `CityMapGraph` | Weighted Graph (Adjacency List) | Model city map, BFS proximity search |
| `HistoricalAVL` | AVL Tree | Store and query resolved incident records |
| `TaskQueue` | Linked List Queue (FIFO) | Vehicle maintenance task scheduling |
| `CommandStack` | Linked List Stack (LIFO) | Dispatcher action history |

## Core Workflow

```
Log Incident → Min-Heap
       ↓
Dispatch (ExtractMin) → BFS on Graph → Find nearest available vehicle
       ↓
Update vehicle status in Hash Table
       ↓
Resolve Incident → Insert record into AVL Tree
```

## Menu Features

**Incident Dispatch** — Log new incident, dispatch next, resolve/close

**Vehicle & Map** — Add/remove vehicles, update status/location, manage graph nodes and edges

**Administration** — View maintenance queue, process tasks, review command history, generate historical report

**Persistence** — Auto-saves all state on exit, auto-loads on startup via `<fstream>`

## Project Structure

```
ERDS/
├── main.cpp
├── IncidentMinHeap.cpp / .h
├── VehicleHashTable.cpp / .h
├── CityMapGraph.cpp / .h
├── HistoricalAVL.cpp / .h
├── TaskQueue.cpp / .h
├── CommandStack.cpp / .h
└── data/               # Saved state files
```

---
