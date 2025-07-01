# PCNSim Setup & Reproduction Guide

## 1. Python Environment Setup

**First run only:**
```bash
python3 -m venv venv
```

**Activate the virtual environment:**
```bash
source venv/bin/activate
```

**Install dependencies:**
```bash
pip install -r requirements.txt
```

---

## 2. Creating Topology and Workload

**Import a dataset:**  
Place your dataset in `/scripts/datasets`.

**Generate topology and workload:**
```bash
cd script
python3 generate_topology_workload.py genTopo -n 10 --lightning
python3 generate_topology_workload.py genWork --n_payments 100 --credit-card
```

---

## 3. Running OMNeT++

**Set up OMNeT++ environment:**
```bash
source setenv
[ -f "$HOME/omnetpp-6.1/setenv" ] && source "$HOME/omnetpp-6.1/setenv"
./configure
make
omnetpp
```

---

## 4. Import Project in OMNeT++

1. Choose working directory.
2. Go to `File > Import`.
3. Select `General > Existing Projects into Workspace`.
4. Set root directory to: `~/BTDA/pcnsim`.
5. Check the `pcnsim` project in the list.
6. Click **Finish**.

---

## 5. Verify Project Files

- In `wpcn-omnet`, ensure the following files exist:
  - `pcn.ini`
  - `pnc.ned`

---

## 6. Start the Project

- Click **Start Project** in OMNeT


## 7. Visualize the results

---

# OMNeT++ Installation Guide

## 1. Download OMNeT++

Download OMNeT++ 6.0 from the [official website](https://omnetpp.org/download/).

## 2. Install Dependencies

Open a terminal and run:
```bash
sudo apt-get update
sudo apt-get install build-essential clang lld gdb bison flex perl \
python3 python3-pip qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools \
libqt5opengl5-dev libxml2-dev zlib1g-dev doxygen graphviz libwebkit2gtk-4.0-37
```

```bash
 
```


## 3. Set Up Python Environment
```bash
python3 -m venv venv
source venv/bin/activate
python3 -m pip install --user --upgrade numpy pandas matplotlib scipy seaborn posix_ipc
```

## 4. Build OMNeT++

Navigate to the OMNeT++ directory and set up the environment:
```bash
cd omnetpp-6.0
source setenv
[ -f "$HOME/omnetpp-6.0/setenv" ] && source "$HOME/omnetpp-6.0/setenv"
./configure
make
```

## 5. Start OMNeT++

```bash
omnetpp
```

# The experiments parameters

**All experiments use:**
- 10 nodes
- 1000 transactions

## 1. Generate a scale-free topology with Lightning channel statistics:
```bash
python3 generate_topology_workload.py genTopo -t scale-free -n 10 --lightning
```

## 2. Generate workloads:

**a) Fixed payment value (200):**
```bash
python3 generate_topology_workload.py genWork --n_payments 1000 --min_payment 200 --max_payment 200
```

**b) Fixed payment value (10):**
```bash
python3 generate_topology_workload.py genWork --n_payments 1000 --min_payment 10 --max_payment 10
```

**c) Realistic payment values from credit card dataset:**
```bash
python3 generate_topology_workload.py genWork --n_payments 1000 --credit_card
```
