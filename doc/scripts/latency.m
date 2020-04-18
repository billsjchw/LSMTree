ops = {'PUT', 'GET (seq)', 'GET (rnd)', 'DEL'};
data = [0.0979; 0.0498; 0.2570; 0.0075];
bar(data);
ylabel('latency (ms/op)', 'Interpreter', 'latex');
set(gca, 'XTickLabel', ops, 'TickLabelInterpreter', 'LaTex');
