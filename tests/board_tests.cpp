
#include <boost/ut.hpp>

#include "board.hpp"

int main()
{
  using namespace boost::ut;

  "board_init"_test = [] {
    Board board{};
    expect(board.getState() == 0);
  };
}
