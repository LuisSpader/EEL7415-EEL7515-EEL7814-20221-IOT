import matplotlib.pyplot as plt
import pandas as pd

def plot_distances_SF(path):
    dataframe = pd.read_csv(path)

    for i in range(0, len(dataframe['Distance'])):
        plt.scatter(dataframe['Distance'][i], dataframe['SF'][i],50, color='red')#, label=str(dataframe['BW'][i]))

def plot_hist(path):
    dataframe = pd.read_csv(path)
    plt.hist(dataframe['SF'])