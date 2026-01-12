# Dining Philosophers Problem – Operating Systems Project

## 📌 Project Overview
This project is an implementation of the **Dining Philosophers Problem**, which is a classical synchronization problem in **Operating Systems**.

The goal of the project is to demonstrate:
- thread synchronization,
- mutual exclusion,
- deadlock prevention,
- race condition avoidance,
- and correct usage of mutexes.

Each philosopher is represented as a **thread**, and forks are represented as **mutex locks**.

---

## 🧠 Problem Definition
There are **N philosophers** sitting around a circular table.

- Each philosopher alternates between **thinking**, **eating**, and **sleeping**.
- To eat, a philosopher must hold **two forks**: one on the left and one on the right.
- Forks are shared resources.
- If synchronization is not handled correctly, problems such as **deadlock**, **starvation**, or **race conditions** may occur.

---

## 🧵 Thread & Mutex Design
- Each philosopher runs in its own thread.
- Each fork is protected by a mutex.
- Philosophers must lock both forks before eating.
- Mutexes ensure that no two philosophers can use the same fork at the same time.

---

## 🔒 Synchronization Strategy
To avoid **race conditions**, all shared data (such as meal count, last meal time, and simulation state) is protected using mutexes.

Key synchronization points:
- Fork acquisition
- Printing status messages
- Updating philosopher states
- Death detection

---

## 🚫 Deadlock Prevention
Deadlock is prevented by enforcing an **ordered locking strategy**.

For example:
- Philosophers pick up forks in a consistent order (e.g., lower-index fork first).
- This prevents circular wait conditions.

As a result, the system never reaches a state where all philosophers are waiting indefinitely.

---

## 🍽 Starvation Prevention
- Each philosopher’s last meal time is tracked.
- A monitoring mechanism checks whether a philosopher exceeds the allowed time without eating.
- This ensures fairness and prevents starvation.

---

## ⏱ Time Management
The program uses precise timing to simulate:
- time to eat,
- time to sleep,
- time to die.

Time calculations are done in milliseconds to ensure accuracy.

---

## ⚠️ Philosopher Death Detection
A dedicated monitoring logic checks:
- if the current time − last meal time > time_to_die

If this condition occurs:
- the simulation stops,
- the philosopher is declared dead,
- and all threads are terminated safely.

---

## ▶️ How to Run the Program

```bash
make
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_meals]
