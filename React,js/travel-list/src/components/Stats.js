export default function Stats({ items }) {
  if (!items.length)
    return (
      <p className="stats">
        <em>Start Packing for your vacation!</em>
      </p>
    );

  const numItems = items.length;
  const numPacked = items.filter((item) => item.packed).length;
  const percentage = Math.round((numPacked / numItems) * 100);

  return (
    <footer className="stats">
      <em>
        {percentage === 100
          ? "You are ready for your vacation"
          : `You have ${numItems} items on your list and you have already packed ${" "}
        ${numPacked}(${percentage}%)`}
      </em>
    </footer>
  );
}
