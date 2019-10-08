package ija.ija2018.homework1.board;

public class Board extends java.lang.Object
{

    private int size = 0;
    private Field[][] array;

    public Board(int size)
    {
        this.size = size;
        this.array = new Field[size + 2][size + 2];
        this.generateField();
        this.setNeighborFields();
    }

    public Field getField(int row, int col)
    {
        if ((row >= 0) && (row <= this.size + 1) && (col >= 0) && (col <= this.size + 1)) return (array[row][col]);
        return null;
    }

    public int getSize()
    {
        return this.size;
    }

    //pomocna funkcia pre inicializaciu
    private void generateField()
    {
        for (int row = 0; row <= this.size + 1; row++) {
            for (int col = 0; col <= this.size + 1; col++) {
                if ((row == 0) || (row == this.size + 1)) {
                    this.array[row][col] = new BorderField();
                } else if ((col == 0) || (col == this.size + 1)) {
                    this.array[row][col] = new BorderField();
                } else {
                    this.array[row][col] = new BoardField(row, col);
                }
            }
        }
    }
    
  //pomocna funkcia pre priradenie susedov
    private void setNeighborFields()
    {
        for (int row = 1; row <= this.size; row++) {
            for (int col = 1; col <= this.size; col++) {
                array[row][col].addNextField(Field.Direction.D,  this.array[row]	[col - 1]);
                array[row][col].addNextField(Field.Direction.L,  this.array[row - 1][col]);
                array[row][col].addNextField(Field.Direction.LD, this.array[row - 1][col - 1]);
                array[row][col].addNextField(Field.Direction.LU, this.array[row - 1][col + 1]);
                array[row][col].addNextField(Field.Direction.R,  this.array[row + 1][col]);
                array[row][col].addNextField(Field.Direction.RD, this.array[row + 1][col - 1]);
                array[row][col].addNextField(Field.Direction.RU, this.array[row + 1][col + 1]);
                array[row][col].addNextField(Field.Direction.U,  this.array[row]	[col + 1]);
            }
        }
    }
}
