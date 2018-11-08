let square = ReasonReact.statelessComponent("Some square");

let make = (~message, _children) => {
  ...square,
  render: self =>{
  <div>
    <button className="square">
      </button>
  </div>}
};
