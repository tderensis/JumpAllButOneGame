#include "board.hpp"
#include <vector>
#include "fmt/core.h"

template <size_t SIZE>
bool solve(const Board<SIZE>& board, std::vector<Board<SIZE>>& boards, int level)
{
    boards[level++] = board;
    if (board.isSolved())
    {
        return true;
    }
    auto next_boards = board.nextBoards();

    for (const auto& board : next_boards)
    {
        if (solve(board, boards, level))
        {
            return true;
        }
    }
    return false;
}
int main()
{
    using BoardType = Board<5>;
    // Create board with a given initial state
    constexpr auto initial_board = BoardType::fromString(
        "    0    "
        "   1 1   "
        "  1 1 1  "
        " 1 1 1 1 "
        "1 1 1 1 1");

    fmt::print("Initial board:\n{}", initial_board);

    std::vector<BoardType> boards(BoardType::size() * (BoardType::size() + 1)/2 - 1);

    bool solved = solve(initial_board, boards, 0);

    if (!solved)
    {
        fmt::print("Could not be solved\n");
        return -1;
    }

    fmt::print("Solved!\n");
    for (auto& b : boards)
    {
        fmt::print("{}", b);
    }
    return 0;
}
