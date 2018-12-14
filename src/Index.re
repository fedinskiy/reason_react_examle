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
  type state = {
    vals:array(player),
    current:player
  };
  type action= Click(int);
  let board = ReasonReact.reducerComponent("the board");

  let make = (~message as _, _children) => {
    ...board,
    initialState: () =>{vals: Array.make(9,None), current: First},
    reducer: (action,state:state) => {
      let i = switch(action) {
        | Click(i)=>i;
      }
      switch(calculateWinner(state.vals)){
        | None=>{
          state.vals[i] = state.current
          ReasonReact.Update({vals: state.vals, current:next(state.current)})}
        | _ => ReasonReact.NoUpdate
      }
    },
    render: self =>{
      let winner = calculateWinner(self.state.vals);
      let status = switch(winner){
        | None=>"Next player: "++show(self.state.current);
        | _ => "Winner: "++show(winner)};

      let renderSquare=(i:int)=>{
        <Square
                value={self.state.vals[i]}
                onClick={_event=>self.send(Click(i))}
                message=""/>
      };
      <div>
      <div className="status">{ReasonReact.string(status)}</div>
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
  let game = ReasonReact.statelessComponent("some game");

  let make = (~message as _, _children) => {
    ...game,
    render: _self =>{
      <div className="game">
        <div className="game-board">
          <Board message=""/>
          </div>
          <div className="game-info">
          <div>{ReasonReact.null}</div>
          <ol>{ReasonReact.null}</ol>
          </div>
          </div>
        }
      };
};


ReactDOMRe.renderToElementWithId(<Game message="Hi!" />, "index1");
