*This project has been created as part of the 42 curriculum by tafujise*

# Philosophers

## Description

This project explores the classical Dining Philosophers problem to introduce the fundamentals of concurrent programming in C. 

The goal is to simulate philosophers who alternately eat, sleep, and think while sharing limited resources (forks) without causing data races, deadlocks, or starvation.

In the mandatory part, each philosopher is implemented as a separate thread, and forks are protected using mutexes to ensure safe access to shared resources.

## Instructions
```
make
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

```

## Resources
### Core Concepts
- **Dining Philosophers Problem**  
  Overview of the classical synchronization problem.
  https://en.wikipedia.org/wiki/Dining_philosophers_problem

- **Starvation (Computer Science)**  
  Explanation of starvation and its relation to scheduling and resource allocation.  
  https://en.wikipedia.org/wiki/Starvation_(computer_science)

- **Deadlock (Computer Science)**
  Explanation of deadlock.
  https://en.wikipedia.org/wiki/Deadlock_(computer_science)

- **Coffman Conditions**  
  The four necessary conditions for deadlock.  
  https://en.wikipedia.org/wiki/Deadlock_(computer_science)#Individually_necessary_and_jointly_sufficient_conditions_for_deadlock

- **Advanced Programming in the UNIX Environment – Chapter 12**  
  Thread programming and synchronization primitives.

### Use of AI
AI tools were used for research.
