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
