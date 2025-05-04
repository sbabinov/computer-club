#include <iostream>
#include <fstream>
#include "club.hpp"
#include "events.hpp"
#include "structs.hpp"
#include <sstream>
#include <memory>
#include <vector>
#include <utility>

int main(int argc, char* argv[])
{
  using structs::Time;
  using events::ClientEvent;

  if (argc != 2)
  {
    std::cerr << "Invalid parameters" << '\n';
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "File doesn't exist" << '\n';
    return 1;
  }
  std::string line = "";
  std::getline(file, line);
  std::istringstream iss(line);
  int nTables = 0;
  iss >> nTables;
  if (!iss || (nTables <= 0))
  {
    std::cerr << line << '\n';
    return 1;
  }

  std::getline(file, line);
  iss = std::istringstream(line);
  Time openTime{};
  Time closeTime{};
  iss >> openTime >> closeTime;
  if (!iss)
  {
    std::cerr << line << '\n';
    return 1;
  }

  std::getline(file, line);
  iss = std::istringstream(line);
  int price = 0;
  iss >> price;
  if (!iss || (price <= 0))
  {
    std::cerr << line << '\n';
    return 1;
  }

  club::ComputerClub cl(nTables, {openTime, closeTime}, price, &std::cout);
  std::vector< std::unique_ptr< ClientEvent > > events;
  events.push_back(std::make_unique< ClientEvent >());
  while (std::getline(file, line))
  {
    iss = std::istringstream(line);
    auto event = std::make_unique< ClientEvent >();
    iss >> event;
    if ((!iss) || (event->getTime() <= (events.back()->getTime())))
    {
      std::cerr << line << '\n';
      return 1;
    }
    events.push_back(std::move(event));
  }

  for (auto it = events.begin() + 1; it != events.end(); ++it)
  {
    cl.processEvent(**it);
  }

  return 0;
}
