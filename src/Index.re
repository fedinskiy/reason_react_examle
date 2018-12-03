[@bs.val] external alert : string => unit = "alert";

type player=First|Second|None;
let show = (player:player):string=>{
  switch(player){
    | First => "X"
    | Second => "O"
    | None => ""
  }
}

let nextMove= (player)=>
  switch(player) {
    | First => Second
    | _ => First
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
    next:player
  };
  type action= Click(int);
  let board = ReasonReact.reducerComponent("the board");

  let make = (~message as _, _children) => {
    ...board,
    initialState: () =>{vals: Array.make(9,None), next: First},
    reducer: (action,state:state) => {
      let i = switch(action) {
        | Click(i)=>i;
      }
      let current=state.vals
      current[i] = nextMove(current[i])
      ReasonReact.Update({...state, vals: current})
    },
    render: self =>{
      let status = "Next player: X";

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
