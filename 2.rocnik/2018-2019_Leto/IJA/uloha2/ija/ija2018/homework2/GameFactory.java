package ija.ija2018.homework2;

import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.game.Board;

public class GameFactory {

	public static Game createChessGame(Board board) {
		// TODO Auto-generated method stub
		
		board.addFigure(1, 1, "V", "W");
		board.addFigure(8, 1, "V", "W");
		for (int i = 1; i <= 8; i++)
			board.addFigure(i, 2, "P", "W");
		
		board.addFigure(1, 8, "V", "B");
		board.addFigure(8, 8, "V", "B");
		for (int i = 1; i <= 8; i++)
			board.addFigure(i, 7, "P", "B");
		
		board.game_type = "S";
		
		return board;
	}

	public static Game createCheckersGame(Board board) {
		for (int i = 1; i <= 8; i+=2)
			board.addFigure(i, 1, "P", "W");
		for (int i = 2; i <= 8; i+=2)
			board.addFigure(i, 2, "P", "W");
		
		for (int i = 1; i <= 8; i+=2)
			board.addFigure(8, i, "P", "B");
		for (int i = 2; i <= 8; i+=2)
			board.addFigure(7, i, "P", "B");
		
		board.game_type = "D";
		return board;
	}


}
