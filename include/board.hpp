#pragma once

#include <vector>
#if __has_include("fmt/core.h")
#include "fmt/core.h"
#endif

template <typename T>
constexpr bool isTriangular(T x)
{
    if (x <= 0)
    {
        return false;
    }

    // A Triangular number is the sum of consecutive natural numbers
    for (T sum = 0, n = 1; sum <= x; ++n)
    {
        if (sum > (std::numeric_limits<T>::max() - n))
        {
            break;
        }
        sum = sum + n;
        if (sum == x)
        {
            return true;
        }
    }

    return false;
}

template <size_t SIZE = 5>
struct Board
{
    static constexpr size_t size()    { return SIZE; }
    static constexpr size_t numPegs() { return SIZE * (SIZE + 1)/2; }

    //     1
    //    0 1
    //   0 0 0
    //  1 0 1 0
    // 1 1 0 0 0
    // = 0b00011 0101 000 10 1 = 0b000'1101'0100'0101 = 0x0D45
    static constexpr Board fromString(std::string_view sv)
    {
        unsigned initial_state = 0;
        unsigned position = 0;
        for (auto v : sv)
        {
            if (v == ' ' || v == '\t' || v == '\n' || v == '\r')
            {
                continue;
            }

            if (v == '0')
            {
                // Nothing to do. Just increment position.
                ++position;
            }
            else
            {
                // Any other character is considered filled.
                initial_state |= 1U << position;
                ++position;
            }
        }
	//static_assert(position == numPegs());

        return Board(initial_state);
    }

    constexpr Board() : Board(0) {};

    explicit constexpr Board(unsigned initial_state) :
        m_state{initial_state}
    {};



    Board jump(
        const Board& board, unsigned position,
        unsigned neighbor_pos, unsigned jump_pos) const
    {
        auto state = board.getState();
        auto next_state = (state |  (1U << jump_pos));
        next_state = (next_state & ~(1U << neighbor_pos));
        next_state = (next_state & ~(1U << position));
        return Board{next_state};
    }
    std::vector<Board> nextBoards() const
    {
        std::vector<Board> next_boards;
        unsigned position = 0;
        unsigned row = 0;
        unsigned col = 0;
        while (position < SIZE * (SIZE + 1) / 2)
        {
            if (!(m_state & (1U << position)))
            {
                ++position;
                if (isTriangular(position))
                {
                    col = 0;
                    ++row;
                }
                else
                {
                    ++col;
                }
                continue;
            }

            // upper left
            if (row > 1 && col > 1)
            {
                unsigned neighbor_pos = position - (row + 1);
                if (m_state & (1U << neighbor_pos))
                {
                    unsigned jump_pos = neighbor_pos - row;
                    if (!(m_state & (1U << jump_pos)))
                    {
                        // Clear the neighbor and set the jump position
                        auto state = jump(
                            *this, position, neighbor_pos, jump_pos);
                        next_boards.push_back(Board(state));
                    }
                }
            }
            // upper right
            if (row > 1 && (col + 1 < row))
            {
                unsigned neighbor_pos = position - row;
                if (m_state & (1U << neighbor_pos))
                {
                    unsigned jump_pos = neighbor_pos - row + 1;
                    if (!(m_state & (1U << jump_pos)))
                    {
                        // Clear the neighbor and set the jump position
                        auto state = jump(
                            *this, position, neighbor_pos, jump_pos);
                        next_boards.push_back(Board(state));
                    }
                }
            }

            // left
            if (row > 1 && col > 1)
            {
                unsigned neighbor_pos = position - 1;
                if (m_state & (1U << neighbor_pos))
                {

                    unsigned jump_pos = neighbor_pos - 1;
                    if (!(m_state & (1U << jump_pos)))
                    {
                        // Clear the neighbor and set the jump position
                        auto state = jump(
					  *this, position, neighbor_pos, jump_pos);
                        next_boards.push_back(Board(state));
                    }
                }
            }

            // right
            if (row > 1 && (col + 1 < row))
            {
                unsigned neighbor_pos = position + 1;
                if (m_state & (1U << neighbor_pos))
                {
                    unsigned jump_pos = neighbor_pos + 1;
                    if (!(m_state & (1U << jump_pos)))
                    {
                        // Clear the neighbor and set the jump position
                        auto state = jump(
                            *this, position, neighbor_pos, jump_pos);
                        next_boards.push_back(Board(state));
                    }
                }
            }

            // lower left
            if (row < SIZE - 2)
            {
                unsigned neighbor_pos = position + (row + 1);
                if (m_state & (1U << neighbor_pos))
                {
                    unsigned jump_pos = neighbor_pos + row + 2;
                    if (!(m_state & (1U << jump_pos)))
                    {
                        // Clear the neighbor and set the jump position
                        auto state = jump(
                            *this, position, neighbor_pos, jump_pos);
                        next_boards.push_back(Board(state));
                    }
                }
            }

            // lower right
            if (row < SIZE - 2)
            {
                unsigned neighbor_pos = position + (row + 2);
                if (m_state & (1U << neighbor_pos))
                {
                    unsigned jump_pos = neighbor_pos + row + 3;
                    if (!(m_state & (1U << jump_pos)))
                    {
                        // Clear the neighbor and set the jump position
                        auto state = jump(
                            *this, position, neighbor_pos, jump_pos);
                        next_boards.push_back(Board(state));
                    }
                }
            }

            ++position;

            if (isTriangular(position))
            {
                col = 0;
                ++row;
            }
            else
            {
                ++col;
            }
        }
        return next_boards;
    }

    Board rotateClockwise()
    {
      // Before:
      //         0
      //       1   2
      //     3   4   5
      //   6   7   8   9
      //10  11  12  13  14
      // 0  (0, 0) -> 14 (4, 4)
      // 1  (1, 0) -> 9  (3, 3)
      // 2  (1, 1) -> 13 (4, 3)
      // 3  (2, 0) -> 5  (2, 2)
      // 4  (2, 1) -> 8  (3, 2)
      // 5  (2, 2) -> 12 (4, 2)
      // 6  (3, 0) -> 2  (1, 1)
      // 7  (3, 1) -> 4  (2, 1)
      // 8  (3, 2) -> 7  (3, 1)
      // 9  (3, 3) -> 11 (4, 1)
      // 10 (4, 0) -> 0  (0, 0)
      // 11 (4, 1) -> 1  (1, 0)
      // 12 (4, 2) -> 3  (2, 0)
      // 13 (4, 3) -> 6  (3, 0)
      // 14 (4, 4) -> 10 (4, 0)
	size_t nextRow = SIZE - 1;
        size_t nextCol = SIZE - 1;
	unsigned state = 0;
	for (size_t position = 0; position < numPegs(); ++position)
	  {
	    if (isTriangular(position))
	      {
		--nextCol;
		nextRow = nextCol;
	      }

	    // Convert row/col to position and move the bit
	    if (m_state & (1U <<position))
	      {
		auto new_position = toPosition(nextRow, nextCol);
		state |= 1U << new_position;
	      }
	    ++nextRow;
	  }

	    return {state};
    }

  size_t toPosition(size_t row, size_t col)
  {
    return 0;
  }

    unsigned getState() const { return m_state; };

    bool isSolved() const {
        return (m_state & (m_state - 1)) == 0;
    }
private:
    unsigned m_state;
};

#if __has_include("fmt/core.h")
template <size_t SIZE> struct fmt::formatter<Board<SIZE>> {

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }
    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext>
    auto format(const Board<SIZE>& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        // ctx.out() is an output iterator to write to.
        auto state = p.getState();

        int position = 0;
        for (size_t i = 0; i < SIZE; ++i)
        {
            // leading spaces
            fmt::format_to(ctx.out(), "{: >{}}", "", SIZE - 1 - i);

            for (size_t j = 0; j < i + 1; ++j)
            {
                fmt::format_to(ctx.out(), "{} ",
                    (state & (1U << position++)) ? '1' : '0');
            }
            // trailing spaces
            fmt::format_to(ctx.out(), "{: >{}}\n", "", SIZE - 1 - i);

        }
        return fmt::format_to(ctx.out(), "");
    }
};
#endif
