function FinishScreen({ maxPossiblePoints, points, highscore, dispatch }) {
  const percentage = (points / maxPossiblePoints) * 100;
  return (
    <>
      <p className="result">
        {" "}
        You scored <strong>{points}</strong> out of {maxPossiblePoints} (
        {Math.ceil(percentage)} %)
      </p>
      <p className="highscore">(Highscore : {highscore} ponts)</p>
      <button className="btn" onClick={() => dispatch({ type: "restart" })}>
        Restart
      </button>
    </>
  );
}

export default FinishScreen;
