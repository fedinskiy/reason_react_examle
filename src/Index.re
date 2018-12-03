[@bs.val] external alert : string => unit = "alert";

type clicked=First|Second|None;
let show = (player:clicked):string=>{
  switch(player){
    | First => "X"
    | Second => "O"
    | None => ""
  }
}
module Square {
  type state = {
    wasClicked: clicked
  };

  type action = Click ;

  let square = ReasonReact.reducerComponent("Some square");

  let make = (~message, ~value, _children) => {
    ...square,

    initialState: () => {wasClicked: None},
    reducer: (action:action, state:state) => switch(state.wasClicked) {
      | First => ReasonReact.Update({wasClicked: Second})
      | _ => ReasonReact.Update({wasClicked: First})
    },

    render: self =>{
    <div>
      <button className="square" onClick={_event=>self.send(Click)}>
          {ReasonReact.string(show(self.state.wasClicked))}
        </button>
    </div>
    }
  };
};

module Board {
  type state = {
    vals:array(clicked)
  };
  type action= Click;
  let board = ReasonReact.reducerComponent("the board");

  let make = (~message, _children) => {
    ...board,
    initialState: () =>{vals: Array.make(9,None)},
    reducer: ((),state:state) => ReasonReact.NoUpdate,
    render: self =>{
      let status = "Next player: X";
      let renderSquare=(i:int)=>{
        <Square value={self.state.vals[i]} message=""/>
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

  let make = (~message, _children) => {
    ...game,
    render: self =>{
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
