#
# This file is the script file for generating histogram
#
# Copyright (C) 2024-2025 Cédrick Austa <cedrick.austa@ulb.be>
# Copyright (C) 2020 Shuwen Deng <shuwen.deng@yale.edu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
#
#

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import sys

params = {
    'legend.title_fontsize': 15,
    'legend.fontsize': 14,
    'legend.handlelength': 2,
    #'legend.frameon': False,
    'axes.titlesize'      : 15,
    'axes.labelsize'      : 15,
    'xtick.labelsize' : 14,
    'ytick.labelsize' : 14
}
plt.rcParams.update(params)

def expand_df(table):
    freq_dict = {}
    for col in table.columns:
        if col != "Cycles":
            freq_dict[col] = table.loc[table.index.repeat(table[col]),"Cycles"]
            freq_dict[col] = freq_dict[col].to_numpy()

    for col in table.columns:
        if col == "Cycles":
            continue
        else:
            size = freq_dict[col].size
            if size < 10000:
                if size == 0:
                    del freq_dict[col]
                else:
                    print(col)
    return pd.DataFrame(freq_dict)

if __name__ == "__main__":
    input_file_name = sys.argv[1]
    output_file_name = sys.argv[2]
    results = pd.read_csv(input_file_name, sep=",", header=[0,1])
    results = results.loc[:,results.columns[:-1]]
    cycles = pd.DataFrame(results[:]["Cycles"])

    read = pd.DataFrame(results["READ"])
    read["Cycles"] = cycles
    read = expand_df(read)
    read["Operation"] = "READ"

    write = pd.DataFrame(results["WRITE"])
    write["Cycles"] = cycles
    write = expand_df(write)
    write["Operation"] = "WRITE"

    if "FLUSH" in results.columns:
        flush = pd.DataFrame(results["FLUSH"])
        flush["Cycles"] = cycles
        flush = expand_df(flush)
        flush["Operation"] = "FLUSH"
        table = pd.concat([read, write, flush], axis=0)
    else:
        table = pd.concat([read, write], axis=0)

    table = pd.melt(table,
                    id_vars=["Operation"],
                    var_name="Timing type",
                    value_name="Cycles")

    palette = sns.color_palette("blend:#ff3f3f,#0171CF,#12d15c", 3)

    fig, ax = plt.subplots(figsize=(9, 6))
    sns.barplot(
            x="Cycles", y="Timing type",
            data=table, hue="Operation",
            orient='h',
            errorbar=("pi",95),
            ax=ax, palette=palette, alpha=.6
    )
    x_label = "Clock cycles"
    y_label = "Timing types"
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.xaxis.grid()
    plt.tight_layout()
    fig.savefig(f"{output_file_name}.eps", dpi=1000)#, format="png")
    plt.close(fig)
