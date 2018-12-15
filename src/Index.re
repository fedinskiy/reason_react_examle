[@bs.val] external alert : string => unit = "alert";

type player=First|Second|None;
let show = (player:player):string=>{
  switch(player){
    | First => "X"
    | Second => "O"
    | None => ""
  }
}

let rec getFirstN=(list, n:int)=>{
  switch(list,n){
    | ([],_)=>[]
    | (_,0)=>[]
    | ([first, ...rest],_)=>[first, ...getFirstN(rest,n-1)]
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
  player: player,
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
let rec shrinkToLength=(list:list('a), length:int)=>{
  switch(List.length(list)==length){
    | true => list;
    | false => shrinkToLength(List.tl(list), length);
  }
}
module Game {
  type state = {
    history: list(turn),
    shownTurn: int
  };
  type action = Click(int)|Step(int);
  let game = ReasonReact.reducerComponent("some game");

  let processButtonPress=(button:int,state:state)=>{
    let historyTillNow = shrinkToLength(state.history, state.shownTurn+1)
    let [turn, ..._] = historyTillNow

    switch(calculateWinner(turn.vals), turn.vals[button]){
      | (None, None) =>{
        let values=Array.copy(turn.vals)
        values[button] = turn.player
        ReasonReact.Update({
          shownTurn: state.shownTurn+1,
          history: [{
            vals: values,
            num: turn.num+1,
            player:next(turn.player)},
            ...historyTillNow],})}
      | _ => ReasonReact.NoUpdate
    }
  };
  let processStepSwitch=(step:int,state:state)=>{
    ReasonReact.Update({...state, shownTurn:step})
  }
  let make = (~message as _, _children) => {
    ...game,
    initialState: () => {history:[{vals:Array.make(9,None),player: First, num:0}],shownTurn:0},
    reducer: (action,state:state) => {
      switch(action) {
        | Click(i)=>processButtonPress(i,state);
        | Step(i)=>processStepSwitch(i,state);
      }
    },
    render: self =>{
      let current = {
        let hist = self.state.history
        let n = List.length(hist)-self.state.shownTurn
        List.nth(hist,n-1)
      }
      let winner = calculateWinner(current.vals);
      let moves = self.state.history->ListLabels.map(~f=(turn)=>{
        let desc = turn.num==0
              ? "Go to game start"
              : "Go to move #"++string_of_int(turn.num);

          <li>
            <button
                    key={string_of_int(turn.num)}
                    onClick={_event=>self.send(Step(turn.num))}>
                    {ReasonReact.string(desc)}
            </button>
          </li>
      })->ArrayLabels.of_list;
      let status = switch(winner){
        | None => "Next player: "++show(current.player);
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
