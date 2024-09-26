import React from "react";
import ReactDOM from "react-dom/client";
import "./index.css";
import App from "./App-v1";
// import StarRating from "./StarRating";
import { useState } from "react";

// function SetStateExternal() {
//   const [movieRating, setMovieRating] = useState(0);

//   return (
//     <div>
//       <StarRating color="green" maxRating={10} onSetRating={setMovieRating} />
//       <p>This Movie was rated {movieRating} stars</p>
//     </div>
//   );
// }

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(
  <React.StrictMode>
    <App />
    {/* <StarRating /> */}
    {/* <SetStateExternal /> */}
  </React.StrictMode>
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
