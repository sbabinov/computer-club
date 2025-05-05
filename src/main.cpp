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
  using namespace events;

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
  std::string extra = "";
  std::getline(file, line);
  std::istringstream iss(line);
  int nTables = 0;
  iss >> nTables;
  if (!iss || (nTables <= 0) || (iss >> extra))
  {
    std::cerr << line << '\n';
    return 1;
  }

  std::getline(file, line);
  iss = std::istringstream(line);
  Time openingTime{};
  Time closingTime{};
  iss >> openingTime >> closingTime;
  if (!iss || (iss >> extra) || (openingTime >= closingTime))
  {
    std::cerr << line << '\n';
    return 1;
  }

  std::getline(file, line);
  iss = std::istringstream(line);
  int price = 0;
  iss >> price;
  if (!iss || (price <= 0) || (iss >> extra))
  {
    std::cerr << line << '\n';
    return 1;
  }

  std::vector< std::unique_ptr< ClientEvent > > events;
  events.push_back(std::make_unique< ClientEvent >());
  while (std::getline(file, line))
  {
    iss = std::istringstream(line);
    auto event = std::make_unique< ClientEvent >();
    iss >> event;
    if ((!iss) || (iss >> extra) || (event->getTime() < (events.back()->getTime())))
    {
      std::cerr << line << '\n';
      return 1;
    }
    if (typeid(*event) == typeid(ClientSatEvent))
    {
      size_t table = dynamic_cast< ClientSatEvent& >(*event).getTable();
      if (table > nTables)
      {
        std::cerr << line << '\n';
        return 1;
      }
    }
    events.push_back(std::move(event));
  }
  file.close();

  club::ComputerClub cl(nTables, {openingTime, closingTime}, price, &std::cout);
  for (auto it = events.begin() + 1; it != events.end(); ++it)
  {
    cl.processEvent(**it);
  }

  return 0;
}
