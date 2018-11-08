let board = ReasonReact.statelessComponent("the board");
let renderSquare=(i:int)=>{
  <Square message="i"/>
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
