#include <iostream>
#include <string>
#include <mutex>
#include "ThreadPool.h"


std::mutex coutMutex;

int add(int a, int b)
{
	return a + b;
}

double multiply(double a, double b)
{
	return a * b;
}

std::string makeMessage(std::string name)
{
	return "Hello, " + name;
}

void printMessage(const std::string& message)
{
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << message << std::endl;
}

void printLine(const std::string& text)
{
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << text << std::endl;
}

void lowTask(int id)
{
	printLine("LOW  " + std::to_string(id));
}

void normalTask(int id)
{
	printLine("NORMAL " + std::to_string(id));
}

void highTask(int id)
{
	printLine("HIGH " + std::to_string(id));
}

int main()
{
	ThreadPool pool(4);

	auto f1 = pool.submit(Priority::Low, add, 2, 3);
	auto f2 = pool.submit(Priority::High, multiply, 4.5, 2.0);
	auto f3 = pool.submit(Priority::Normal, makeMessage, std::string("Ilia"));

	pool.submit(Priority::High, printMessage, std::string("High priority void task"));

	printLine("add result: " + std::to_string(f1.get()));
	printLine("multiply result: " + std::to_string(f2.get()));
	printLine("message result: " + f3.get());


	for (int i = 0; i < 30; ++i)
	{
		pool.submit(Priority::Low, lowTask, i);
	}

	for (int i = 0; i < 10; ++i)
	{
		pool.submit(Priority::Normal, normalTask, i);
	}

	for (int i = 0; i < 5; ++i)
	{
		pool.submit(Priority::High, highTask, i);
	}

	return 0;
}