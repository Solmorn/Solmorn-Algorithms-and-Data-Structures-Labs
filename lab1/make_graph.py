import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("graph_data.txt", sep=r"\s+")

df.plot(x="n", y=["arr_time", "list_time"])
plt.xlabel("n")
plt.ylabel("time, DOUBLE")
plt.title("Stack push time: arr_stack and list_stack")
plt.grid(True)
plt.savefig("stack_comparison_graph.svg")