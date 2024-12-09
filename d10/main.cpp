#include <array>    // for array
#include <deque>    // for deque
#include <fstream>  // for basic_ostream, endl, basic_istream, operator<<
#include <iostream> // for cout, cerr
#include <set>      // for set
#include <string>   // for char_traits, basic_string, string
#include <utility>  // for pair
#include <vector>   // for vector

using ElevationMap = std::vector<std::string>;

using Coordinate = int;
using Position = std::pair<Coordinate, Coordinate>;
using Positions = std::vector<Position>;

using Tile = char;

constexpr Tile TRAILHEAD = '0';
constexpr Tile TRAILEND = '9';

constexpr std::array<Position, 4> MOVE_DIRECTIONS = {
    {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

ElevationMap get_elev_map_from_file(const std::string &filepath) {
  std::ifstream in_stream(filepath);

  ElevationMap elev_map;
  for (std::string line; std::getline(in_stream, line); /*BLANK*/) {
    elev_map.push_back(line);
  }

  return elev_map;
}

void print_elev_map(const ElevationMap &elev_map) {
  for (const auto &line : elev_map) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
}

Positions get_trailhead_positions(const ElevationMap &elev_map) {
  Positions trailhead_positions;
  for (Coordinate row{}; row < elev_map.size(); ++row) {
    for (Coordinate col{}; col < elev_map[row].size(); ++col) {
      if (elev_map[row][col] == TRAILHEAD) {
        trailhead_positions.emplace_back(row, col);
      }
    }
  }
  return trailhead_positions;
}

bool is_in_bounds(const ElevationMap &elev_map, const Coordinate row,
                  const Coordinate col) {
  return (row >= 0 && row < elev_map.size() && col >= 0 &&
          col < elev_map.front().size());
}

bool is_elevation_gradually_increasing(const Tile start, const Tile end) {
  return (end - start) == 1;
}

Positions get_trailend_positions(const ElevationMap &elev_map,
                                 const Position &trailhead) {

  Positions trailends;

  std::deque<Position> locations_to_check;

  locations_to_check.push_back(trailhead);

  while (!locations_to_check.empty()) {
    const auto [start_row, start_col] = locations_to_check.front();
    locations_to_check.pop_front();

    if (elev_map[start_row][start_col] == TRAILEND) {
      trailends.emplace_back(start_row, start_col);
      continue;
    }
    for (const auto &[move_row, move_col] : MOVE_DIRECTIONS) {
      const auto new_row = start_row + move_row;
      const auto new_col = start_col + move_col;
      if (!is_in_bounds(elev_map, new_row, new_col)) {
        continue;
      }
      if (!is_elevation_gradually_increasing(elev_map[start_row][start_col],
                                             elev_map[new_row][new_col])) {
        continue;
      }
      // In bounds and we are gradually increasing
      locations_to_check.emplace_back(new_row, new_col);
    }
  }

  return trailends;
}

int count_unique_trailheads(const ElevationMap &elev_map) {
  int accumulator = 0;

  const Positions trailhead_positions = get_trailhead_positions(elev_map);

  for (const auto &trailhead_position : trailhead_positions) {
    const auto trailend_positions =
        get_trailend_positions(elev_map, trailhead_position);
    const std::set<Position> unique_positions(trailend_positions.begin(),
                                              trailend_positions.end());
    accumulator += unique_positions.size();
  }

  return accumulator;
}

int count_unique_trails(const ElevationMap &elev_map) {
  int accumulator = 0;

  const Positions trailhead_positions = get_trailhead_positions(elev_map);

  for (const auto &trailhead_position : trailhead_positions) {
    const auto trailend_positions =
        get_trailend_positions(elev_map, trailhead_position);
    accumulator += trailend_positions.size();
  }

  return accumulator;
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cerr << "Must provide filepath!" << std::endl;
    return -1;
  }

  const auto elev_map = get_elev_map_from_file(argv[1]);

  // print_elev_map(elev_map);

  int accumulator = count_unique_trailheads(elev_map);

  std::cout << "Sum for Part 1: " << accumulator << std::endl;

  accumulator = count_unique_trails(elev_map);

  std::cout << "Sum for Part 2: " << accumulator << std::endl;

  return 0;
}
