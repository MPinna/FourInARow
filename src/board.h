/**
 * TODO naming convention
 */
#include <string>

class Board
{
private:
    /* Notable and costants */
    int DEFAULT_ROWS_NUMBER {6};
    int DEFAULT_COLS_NUMBER {7};
    int WINNING_NUMBER {4};
    int FIRST_WINNING_TURN = {2*WINNING_NUMBER - 1};
public:
    Board(/* args */);
    ~Board();
};

Board::Board(/* args */)
{
}

Board::~Board()
{
}
