[@bs.val] external alert : string => unit = "alert";

type clicked=First|Second|None

module Square {
  type state = {
    clicked
  };

  type action = Click ;

  let square = ReasonReact.reducerComponent("Some square");

  let make = (~message, ~value, _children) => {
    ...square,

    initialState: () => {clicked: None},
    reducer: (action:action, state) => switch(state.clicked) {
      | First => ReasonReact.Update({clicked: Second})
      | _ => ReasonReact.Update({clicked: First})
    },

    render: self =>{
    <div>
      <button className="square" onClick={_event=>self.send(Click)}>
          {switch(self.state.clicked){
              | First => ReasonReact.string("X")
              | Second => ReasonReact.string("Y")
              | None => ReasonReact.string(string_of_int(value))
            }
          }
        </button>
    </div>
    }
  };
};

module Board {
  let board = ReasonReact.statelessComponent("the board");
  let renderSquare=(i:int)=>{
    <Square value={i} message=""/>
  };
  let make = (~message, _children) => {
    ...board,
    render: self =>{
      let status = "Next player: X";
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
      }
    };
};

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
