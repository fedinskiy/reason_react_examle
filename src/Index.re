[@bs.val] external alert : string => unit = "alert";

type player=First|Second|None;
let show = (player:player):string=>{
  switch(player){
    | First => "X"
    | Second => "O"
    | None => ""
  }
}

let next = (player)=>
  switch(player) {
    | First => Second
    | _ => First
  }

type line=(int,int,int);
type turn = {
  vals: array(player),
  current: player,
  num: int
}
let lineWinner = (line:line, board:array(player)):player=>{
    let (x,y,z)=line;
    (board[x]==board[y])&&(board[y]==board[z])
    ? board[x]
    : None
}

let calculateWinner = (board:array(player))=>{
    let lines=[(0,1,2),(3,4,5),(6,7,8),
               (0,3,6),(1,4,7),(2,5,8),
               (0,4,8),(2,4,6)]
    let rec calculate = (lines:list(line)):player => {
      switch (lines){
        | [] => None
        | [current, ...rest] => {
          let winner = lineWinner(current,board)
          switch(winner){
            | None => calculate(rest)
            | _ => winner
          };
        };
      };
    };
    calculate(lines)
}
module Square {
  let square = ReasonReact.statelessComponent("Some square");

  let make = (~message as _, ~value, ~onClick, _children) => {
    ...square,
    render: _self =>{
    <div>
      <button className="square" onClick={onClick}>
          {ReasonReact.string(show(value))}
        </button>
    </div>
    }
  };
};
module Board {
  let board = ReasonReact.statelessComponent("the board");

  let make = (~message as _, ~handler, ~turn, _children) => {
    ...board,
    render: _self =>{
      let renderSquare=(i:int)=>{
        <Square
                value={turn.vals[i]}
                onClick=handler(i)
                message=""/>
      };
      <div>
      <div className="board-row">
        {renderSquare(0)}
        {renderSquare(1)}
        {renderSquare(2)}
        </div>
        <div className="board-row">
        {renderSquare(3)}
        {renderSquare(4)}
        {renderSquare(5)}
        </div>
        <div className="board-row">
        {renderSquare(6)}
        {renderSquare(7)}
        {renderSquare(8)}
        </div>
        </div>
      },
  };
}

module Game {
  type state = {
    history: list(turn),
  };
  type action = Click(int);
  let game = ReasonReact.reducerComponent("some game");

  let make = (~message as _, _children) => {
    ...game,
    initialState: () => {history:[{vals:Array.make(9,None),current: First,num:0}]},
    reducer: (action,state:state) => {
      let i = switch(action) {
        | Click(i)=>i;
      }
      let [turn, ..._] = state.history
      switch(calculateWinner(turn.vals), turn.vals[i]){
        | (None, None) =>{
          turn.vals[i] = turn.current
          ReasonReact.Update({
            history: [{
              vals: turn.vals,
              num: turn.num+1,
              current:next(turn.current)},
              ...state.history],})}
        | _ => ReasonReact.NoUpdate
      }
    },
    render: self =>{
      let [current, ..._] = self.state.history
      let winner = calculateWinner(current.vals);
      let moves = self.state.history->ListLabels.map((turn)=>{
        let desc = turn.num==0
              ? "Go to game start"
              : "Go to move #"++string_of_int(turn.num);

          <li>
            <button>{ReasonReact.string(desc)}</button>
          </li>
      })->ArrayLabels.of_list;
      let status = switch(winner){
        | None=>"Next player: "++show(current.current);
        | _ => "Winner: "++show(winner)};
      <div className="game">
        <div className="game-board">
          <Board
            turn={current}
            handler={(i:int)=>{_event=>self.send(Click(i))}}
            message=""/>
          </div>
          <div className="game-info">
          <br/>
          <div>{ReasonReact.string(status)}</div>
          <ol>...moves</ol>
          </div>
          </div>
        }
      };
};


ReactDOMRe.renderToElementWithId(<Game message="Hi!" />, "index1");
